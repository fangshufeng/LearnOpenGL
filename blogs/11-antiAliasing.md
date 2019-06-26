抗锯齿
=

   * [抗锯齿](#抗锯齿)
      * [准备](#准备)
      * [不做抗锯齿](#不做抗锯齿)
      * [场景一、OpenGL中的MSAA](#场景一opengl中的msaa)
      * [场景二、自定义了帧缓冲如何使用MSAA](#场景二自定义了帧缓冲如何使用msaa)
      * [场景三、自定义了帧缓冲，需要使用MSAA的纹理进行后期处理](#场景三自定义了帧缓冲需要使用msaa的纹理进行后期处理)
      * [场景四、不还原（Resolve）图像-自定义抗锯齿算法](#场景四不还原resolve图像-自定义抗锯齿算法)


## 准备

1. [抗锯齿](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/11%20Anti%20Aliasing/#openglmsaa)

抗锯齿的原理文章讲的很清楚了

## 不做抗锯齿

如果不做抗锯齿边缘会是下面的样子

![](https://learnopengl-cn.github.io/img/04/11/anti_aliasing_zoomed.png)

[代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/17-%E6%8A%97%E9%94%AF%E9%BD%BF/01%E6%B2%A1%E6%9C%89%E5%81%9A%E6%8A%97%E9%94%AF%E9%BD%BF%E5%A4%84%E7%90%86/AntiAliasing.cpp)

## 场景一、OpenGL中的MSAA

上面的代码修改如下


```diff

+ glfwWindowHint(GLFW_SAMPLES, 4);
...

+ glEnable(GL_DEPTH_TEST);
```

效果

![](https://user-images.githubusercontent.com/16829768/60154538-378bef00-981a-11e9-8dd0-da3baa0c31e3.jpg)

## 场景二、自定义了帧缓冲如何使用MSAA

场景一是使用的`glfw`中的`MSAA`，如果自己创建了帧缓冲，那么就需要自己创建`MSAA`了，思路和之前的帧缓冲类似，不过这次是将纹理附件类型改为`GL_TEXTURE_2D_MULTISAMPLE`


```diff
+ GLuint framebuffer;
+ GLuint textureColorBufferMutisampler;
+ void createMutisamplerAndBindTexture() {
    
+     glGenFramebuffers(1,&framebuffer);
+     glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    // 创建多重采样纹理附件
+     glGenTextures(1, &textureColorBufferMutisampler);
+     glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMutisampler);
+     glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGB,SCR_W,SCR_H,GL_TRUE);
+     glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
    // 将纹理附件添加到帧缓冲上
+ glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAM PLE,textureColorBufferMutisampler,0);
    
    // 创建深度和模板渲染缓冲对象
+   GLuint renderBufferObject;
+    glGenRenderbuffers(1,&renderBufferObject);
+     glBindRenderbuffer(GL_RENDERBUFFER,renderBufferObject);
+     glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,SCR_W,SCR_H);
+     glBindRenderbuffer(GL_RENDERBUFFER,0);
    
    // 将渲染缓冲对象绑定到帧缓冲上
+ glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,renderBufferObject);
    
+     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
+         cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
+     glBindFramebuffer(GL_FRAMEBUFFER, 0);
+ }
```

由于我们的片段着色器使用的`sampler2D`采样器，不能直接操作我们的多重采样纹理，所以需要通过方法`glBlitFramebuffer`来还原（Resolve）图片，这个方法的是思想是将一个多重采样纹理位块传送(Blit)到一个没有使用多重采样纹理的`FBO`中，所以如果我们需要`sampler2D`来采样的话，需要再新建一个`FBO`,但这次的`FBO`的纹理附件是一个`2d`附件


```diff
+GLuint intermediaframebuffer;
+GLuint screenTexture;
+void createIntermediaFBO() {
    
+    glGenFramebuffers(1,&intermediaframebuffer);
+    glBindFramebuffer(GL_FRAMEBUFFER,intermediaframebuffer);
    
+    // 创建纹理附件
+    glGenTextures(1, &screenTexture);
+    glBindTexture(GL_TEXTURE_2D,screenTexture);
+    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_W, SCR_H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
+    
+    // 将纹理附件添加到帧缓冲上
+    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

+    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
+        cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
+    glBindFramebuffer(GL_FRAMEBUFFER, 0);
+}
```

然后在多重纹理`FBO`结束后调用`glBlitFramebuffer`进行位块传送


```diff

+   glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
+   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediaframebuffer);
+   glBlitFramebuffer(0, 0, SCR_W, SCR_H, 0, 0, SCR_W, SCR_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);

```

[完整代码](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/17-%E6%8A%97%E9%94%AF%E9%BD%BF/02-%E7%A6%BB%E5%B1%8FMSAA/OffScreenMSAA.cpp)

这样我们就在我们自己定义的帧缓冲中使用了`MSAA`了

## 场景三、自定义了帧缓冲，需要使用MSAA的纹理进行后期处理


现在已经有了一个`intermediaframebuffer`我们可以使用它来做一个后期处理，比如模糊

`off_screen_post_process.fs`代码如下

```diff

+const float offset = 1.0 / 300.0;

void main() {
+    
-    vec3 col = texture(screenTexture, TexCoords).rgb;
-    FragColor = vec4(col, 1.0);
+    vec2 offsets[9] = vec2[](
+                             vec2(-offset,  offset), // 左上
+                             vec2( 0.0f,    offset), // 正上
+                             vec2( offset,  offset), // 右上
+                             vec2(-offset,  0.0f),   // 左
+                             vec2( 0.0f,    0.0f),   // 中
+                             vec2( offset,  0.0f),   // 右
+                             vec2(-offset, -offset), // 左下
+                             vec2( 0.0f,   -offset), // 正下
+                             vec2( offset, -offset)  // 右下
+                             );
+    float kernel[9] = float[](
+                              1.0 / 16, 2.0 / 16, 1.0 / 16,
+                              2.0 / 16, 4.0 / 16, 2.0 / 16,
+                              1.0 / 16, 2.0 / 16, 1.0 / 16
+                              );
+    
+    vec3 sampleTex[9];
+    for(int i = 0; i < 9; i++)
+    {
+        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
+    }
+    vec3 col = vec3(0.0);
+    for(int i = 0; i < 9; i++)
+        col += sampleTex[i] * kernel[i];
+    
+    FragColor = vec4(col, 1.0);
    
}

```

效果如下：

![](https://user-images.githubusercontent.com/16829768/60154541-39ee4900-981a-11e9-977c-2ca87fb8d02e.jpg)


## 场景四、不还原（Resolve）图像-自定义抗锯齿算法

前面说到需要使用`Resolve`的原因是`sampler2D`不能直接采样多重采样纹理，如果要想直接使用多重采样纹理的话就不能使用`sampler2D`，需要使用`sampler2DMS`

`custom_anti_aliasing.fs`


```diff
- uniform sampler2D screenTexture;
+ uniform sampler2DMS screenTexture;

void main() {
-    vec3 col = texture(screenTexture, TexCoords).rgb;
-    FragColor = vec4(col, 1.0);
+    ivec2 texSize = textureSize(screenTexture);
+   vec4 fTexCol = vec4(0.0);
+   for( int i = 0 ; i < 4 ; i++ ) {
+       fTexCol += texelFetch(screenTexture,ivec2(TexCoords * texSize), i);
+   }
+    FragColor = fTexCol / 4;
+}

```

我这里是求四个采样点的平均值，你可以尝试其他的算法。

`CustomAntiAliasing.cpp`

```diff
    ...      
-    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
-    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediaframebuffer);
-    glBlitFramebuffer(0, 0, SCR_W, SCR_H, 0, 0, SCR_W, SCR_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    ...
-    glBindTexture(GL_TEXTURE,screenTexture);        
+    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMutisampler);
    ...
```



