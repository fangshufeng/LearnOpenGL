 * [测试与混合](#测试与混合)
      * [预读](#预读)
      * [一、深度测试](#一深度测试)
         * [1.1 深度测试是解决什么问题的](#11-深度测试是解决什么问题的)
         * [1.2 如何进行深度测试](#12-如何进行深度测试)
         * [1.3 深度值可视化](#13-深度值可视化)
         * [1.4 深度缓存的精度值](#14-深度缓存的精度值)
      * [二、模板测试](#二模板测试)
      * [三、混合](#三混合)
         * [只显示透明的部分](#只显示透明的部分)
         * [部分透明的（比如半透明的纹理）--混合后显示](#部分透明的比如半透明的纹理--混合后显示)

         

# 测试与混合


## 预读

[深度测试](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/)
[模板测试](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/02%20Stencil%20testing/)
[混合](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/03%20Blending/)

##  一、深度测试

### 1.1 深度测试是解决什么问题的

![](https://user-images.githubusercontent.com/16829768/59559731-e28ef280-903b-11e9-89b0-b01e158d11fc.jpg)


图中的方格代表屏幕的像素，当我们观察物体的时候，同一个像素点有物体重合（图中的黄色和橘色）该如何显示呢，到底是显示近处的黄色还是远处的橘色，而深度测试就是告诉OpenGL如何取处理这种情况的。

看下我们的测试场景

![](https://user-images.githubusercontent.com/16829768/59559733-e28ef280-903b-11e9-884c-c34519a7b0ad.jpg)


后添加的地板盖在了物体的上面，而我们是想地板是在物体的下面的，下面开始使用深度测试来解决这个问题。



### 1.2 如何进行深度测试


开启深度测试（默认是关闭的）


```
glEnable(GL_DEPTH_TEST );
```

告诉`OpenGL`应该如何处理重合的地方，是通过函数`glDepthFunc`，这个函数的入参就是具体的处理方式


| 函数 | 描述 |
| --- | :-: |
| GL_ALWAYS | 永远通过深度测试 |
| GL_NEVER | 永远不通过深度测试 |
| GL_LESS | 在片段深度值 < 缓冲的深度值时通过测试 |
| GL_EQUAL | 在片段深度值 = 缓冲区的深度值时通过测试 |
| GL_LEQUAL | 在片段深度值 <= 缓冲区的深度值时通过测试 |
| GL_GREATER | 在片段深度值 > 缓冲区的深度值时通过测试 |
| GL_NOTEQUAL | 在片段深度值 != 缓冲区的深度值时通过测试 |
| GL_GEQUAL | 在片段深度值 >= 缓冲区的深度值时通过测试 |


默认是`GL_LESS`，上面的场景，我们想显示离观察者近的，所以使用默认值即可


```
 glDepthFunc(GL_LESS);
```

还有一个不要忘记，每次渲染前要清空缓存区


```
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

结果如下

![](https://user-images.githubusercontent.com/16829768/59559735-e3278900-903b-11e9-89dc-7ada336db37c.jpg)


`OpenGL`是如何做到这点的呢，其实是维护了一个深度缓存值，这个缓冲记录着每个像素的深度值，一旦测试通过就会更新这个深度值。


一个像素显示之前会去拿自己的深度值`gl_FragCoord.z`和深度值去比较，如果测试通过就会更新深度值，然后显示，如果不通过则会丢弃该片段。

### 1.3 深度值可视化

既然像素的深度值是在`gl_FragCoord.z`，那么我们可以输出`gl_FragCoord.z`来查看物体的深度值


```
void main() {
    //FragColor = vec4(texture(texture1, TexCoord).rgb,1.0f);
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
```

输出结果

![](https://user-images.githubusercontent.com/16829768/59559738-e3c01f80-903b-11e9-9032-c02a88381297.jpg)


嗯竟然是纯白的，按照我们的理解，地板离我们的观察应该近一些，应该值要小一些，那么颜色应该暗一些，那为什么都是白的呢，难道我们理解错了？ 答案当然不是的，这个需要看下深度缓存的精度值

### 1.4 深度缓存的精度值

造成上面的现象的原因是我们的深度值不是和距离不是线性的关系而是下面这张图

![](https://learnopengl-cn.github.io/img/04/01/depth_non_linear_graph.png)



它在z值很小的时候有很高的精度，而z值很大的时候有较低的精度。片段的深度值会随着距离迅速增加，所以几乎所有的顶点的深度值都是接近于1.0的。从而如果我们慢慢接近物体，物体到一定距离后迅速变成黑色，

![](https://user-images.githubusercontent.com/16829768/59559739-e3c01f80-903b-11e9-86eb-ae2603a36d09.jpg)


当然也是可以变成线性关系的


```
#version 330 core
out vec4 FragColor;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    FragColor = vec4(vec3(depth), 1.0);
}

```

![](https://user-images.githubusercontent.com/16829768/59559740-e458b600-903b-11e9-8da9-244cf184885b.jpg)


[深度测试代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/08-%E6%B7%B1%E5%BA%A6%E6%B5%8B%E8%AF%95/DepthTest.cpp)



##  二、模板测试

模板测试类似一个遮罩，由你来决定是保留还是丢弃这个遮罩。

比如我们的场景想添加一个长方形的框，让我们的视图范围只有这个长方形大小

![](https://user-images.githubusercontent.com/16829768/59559743-e4f14c80-903b-11e9-820a-4ad773d904a4.jpg)


效果如下，丢弃了除了长方形的框的片段。

![](https://user-images.githubusercontent.com/16829768/59559745-e589e300-903b-11e9-9e57-528531ffe023.jpg)

[矩形窗口代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/08-%E6%B7%B1%E5%BA%A6%E6%B5%8B%E8%AF%95/StencilCutTest.cpp)

模板测试的一般都是按照以下步骤进行的：

1. 启用模板缓冲的写入。
2. 渲染你的遮罩（任何形状的物体），更新模板缓冲的内容。
3. 禁用模板缓冲的写入；
4. 渲染（其它）物体，这次根据模板缓冲的内容丢弃或者保留特定的片段。


再来看个例子，给我们的场景加个边框

![](https://user-images.githubusercontent.com/16829768/59559776-f4709580-903b-11e9-8b2e-c6f196ed324d.jpg)


步骤如下：

1. 绘制地板，但是不要开启模板测试，保证绘制地板的时候不会更新模板缓冲；
2. 开启模板缓冲写入，正常绘制我们的2个立方体，将绘制到的片段的模板值都变成1；
3. 将我们的物体稍微放大一点，然后绘制不等于1的模板值

代码如下


```
   // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //禁止模板写入
        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        glActiveTexture(GL_TEXTURE0);
        // 地板
        drawPlane(shader);

        glStencilFunc(GL_ALWAYS, 1 ,0xFF);
        glStencilMask(0xFF);
        drawCubes(shader);

        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1 ,0xFF);
        drawBorderCubes(borderShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
```

结果如下

![](https://user-images.githubusercontent.com/16829768/59559777-f4709580-903b-11e9-99a4-9af6f8e1ff58.jpg)


会发现底部的边框不见了，这是因为深度测试导致丢弃了该区域，所以要在绘制边框之前关闭深度测试,画完边框之后再恢复深度测试


```
glStencilMask(0x00);
glStencilFunc(GL_NOTEQUAL, 1 ,0xFF);
glDisable(GL_DEPTH_TEST);
drawBorderCubes(borderShader);
glEnable(GL_DEPTH_TEST);
```

边框已经正常了
![](https://user-images.githubusercontent.com/16829768/59559778-f5092c00-903b-11e9-925c-e5f24598b5a4.jpg)


当我们移动场景会发现以下的bug
![](https://user-images.githubusercontent.com/16829768/59559779-f5092c00-903b-11e9-9928-08f9f5f598b0.jpg)

这是因为在画完边框之后要恢复模板写入，因为所有立方体到的地方模板值都会变成1


```
glStencilFunc(GL_NOTEQUAL, 1 ,0xFF);
glDisable(GL_DEPTH_TEST);
drawBorderCubes(borderShader);
glStencilMask(0xFF);//  恢复模板写入
glEnable(GL_DEPTH_TEST);
        
```

这样我们随意挪动场景，我们的边框都会表现正常了，完整代码如下


```cpp
 while (glfwWindowShouldClose(window) == false) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glm::mat4  view = camera.GetViewMatrix();
        glm::mat4  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        borderShader.use();
        borderShader.setMat4("view", view);
        borderShader.setMat4("projection", projection);
     
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //禁止模板写入
        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        glActiveTexture(GL_TEXTURE0);
        // 地板
        drawPlane(shader);

        glStencilFunc(GL_ALWAYS, 1 ,0xFF);
        glStencilMask(0xFF);
        drawCubes(shader);

        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1 ,0xFF);
        glDisable(GL_DEPTH_TEST);
        drawBorderCubes(borderShader);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
```

[边框代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/08-%E6%B7%B1%E5%BA%A6%E6%B5%8B%E8%AF%95/StencilTest.cpp)

## 三、混合


在通过了模板和深度测试以后就会来到混合（如果开启的话），一般是针对那些有透明度的纹理的


### 只显示透明的部分

现在我们要画一个下面的样子

![](https://user-images.githubusercontent.com/16829768/59559782-f63a5900-903b-11e9-83f9-b7ff387d23c5.jpg)

要想画出场景需要先订出顶点，立方体的位置是由一个以坐标轴为中心经过2个平移矩阵得到的


```
1. glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
2. glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
```

以右边的立方体为例，经过`glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));`沿着x轴平移2个单位，最终的位置如下

![](https://user-images.githubusercontent.com/16829768/59559781-f5a1c280-903b-11e9-837b-ee21a4b02bf9.jpg)


接下来是草的位置，一开始的位置如下


```
    float transparentVertices[] = {
        // positions         // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
```

也就是下面的位置

![](https://user-images.githubusercontent.com/16829768/59559783-f63a5900-903b-11e9-84a1-ab4d9a06a613.jpg)


我们要让草在立方体上面 ，并在前面一点点


![](https://user-images.githubusercontent.com/16829768/59559785-f76b8600-903b-11e9-9a6e-0f1489d0c9cd.jpg)


其他的点以此类推（图太难画了。。。）

回到一开始那个草的场景后面有一层白的背景，那是因为我们只是把场景渲染出来了还没有做任何处理，由于我们的草的纹理是有透明度的，我们只需丢弃掉透明度低于某个值的即可


```
void main() {
    
   vec4 textu =  texture(texture1, TexCoord);
    
    if (textu.a < 0.5) {
        discard;
    }
    
    FragColor = vec4(texture(texture1, TexCoord).rgb,1.0f);
}

```

这下就正常了

![](https://user-images.githubusercontent.com/16829768/59559786-f76b8600-903b-11e9-8295-b1e0db9e8b49.jpg)

[草的代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/10-%E6%B7%B7%E5%90%88c/Blend.cpp)




### 部分透明的（比如半透明的纹理）--混合后显示



实际中，纹理并不只是透明和不透明这个单一，这个时候就需要一个算法计算出最后该显示的颜色，这就需要使用我们的混合技术，怎么混合的也很简单，请看原文，接下来我们将绘制下面的场景


先把窗口应用到我们的场景中，替代草的位置,此时还没有做任何操作，

![](https://user-images.githubusercontent.com/16829768/59559787-f8041c80-903b-11e9-96f3-b02d5ca3a225.jpg)


开启混合功能


```
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
```


效果如下

![](https://user-images.githubusercontent.com/16829768/59559788-f8041c80-903b-11e9-9f38-30611db3fd59.jpg)



看一看出效果已经加上了，但是有点小小的问题，有些地方并没有混合成功，这是什么原因呢，是因为深度测试的影响，我们看下窗口的顶点坐标,分别编号


```
std::vector<glm::vec3> vegetation
{
    glm::vec3(-1.5f, 0.0f, -0.48f), // 1
    glm::vec3( 1.5f, 0.0f, 0.51f), // 5
    glm::vec3( 0.0f, 0.0f, 0.7f),// 3
    glm::vec3(-0.3f, 0.0f, -2.3f), // 2
    glm::vec3 (0.5f, 0.0f, -0.6f) // 4
};
```

对应下图的位置

![](https://user-images.githubusercontent.com/16829768/59559789-f89cb300-903b-11e9-8c86-a2451a177684.jpg)

可以看出近处的`3`比远处的`2`、`4`先画，导致远处的被深度测试丢弃掉了，我们调整下画的顺序，由远到近画，这样就可以通过深度测试了


```
std::vector<glm::vec3> vegetation
{
    glm::vec3(-1.5f, 0.0f, -0.48f), // 1
    glm::vec3( 1.5f, 0.0f, 0.51f), // 5
    glm::vec3(-0.3f, 0.0f, -2.3f), // 2
    glm::vec3 (0.5f, 0.0f, -0.6f), // 4
    glm::vec3( 0.0f, 0.0f, 0.7f),// 3
    
};
```

![](https://user-images.githubusercontent.com/16829768/59559790-f89cb300-903b-11e9-94a9-665c2b6998cf.jpg)


当然在每次按远到近来写顶点是可以解决问题的，但每次这样一个个排序还是挺复杂的，可以借助STL库的map数据结构中。map会自动根据键值(Key)对它的值排序，所以只要我们添加了所有的位置，并以它的距离作为键，它们就会自动根据距离值排序了。


```
std::map<float, glm::vec3> sorted;
for (unsigned int i = 0; i < vegetation.size(); i++) {
   float distance = glm::length(camera.Position - vegetation[i]);
   sorted[distance] = vegetation[i];
}
    
```

然后


```
 // 反向遍历map
   for (std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++) {
       model = glm::mat4();
       model = glm::translate(model, it->second);
       shader.setMat4("model", model);
       glDrawArrays(GL_TRIANGLES, 0, 6);
   }
```


可以看出一样可以计算出的，所以绘制的顺序大致如下：

1. 先绘制所有不透明的物体。
2. 对所有透明的物体排序。
3. 按顺序绘制所有透明的物体



[透明度代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/10-%E6%B7%B7%E5%90%88c/Blend_alpha.cpp)

