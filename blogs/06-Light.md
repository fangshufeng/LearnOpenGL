# 光照

   * [光照](#光照)
      * [一、物体最后呈现的颜色和什么有关](#一物体最后呈现的颜色和什么有关)
      * [二、冯氏光照模型](#二冯氏光照模型)
         * [2.1 基本光照](#21-基本光照)
         * [2.2 物体的材质](#22-物体的材质)
            * [2.2.1 光照模型](#221-光照模型)
            * [2.2.2 材质模型](#222-材质模型)
            * [2.2.3 各个分量的颜色](#223-各个分量的颜色)
            * [2.2.4 一个简单的例子](#224-一个简单的例子)
         * [2.3 物体的贴图](#23-物体的贴图)
            * [漫反射贴图](#漫反射贴图)
            * [镜面光贴图](#镜面光贴图)
         * [2.4 光源的类型](#24-光源的类型)
            * [平行光](#平行光)
            * [点光源](#点光源)
            * [聚光](#聚光)
               * [补充](#补充)
         * [2.5 多个光源](#25-多个光源)

## 一、物体最后呈现的颜色和什么有关

物体最后呈现的颜色大致分为 `物体本身的颜色` + `光的颜色`

比如一个物体的颜色是`glm::vec3 toyColor(1.0f, 0.5f, 0.31f);`光的颜色是`glm::vec3 lightColor(1.0f, 1.0f, 1.0f);`，那么最后反射到人眼看到的颜色是`(1.0f, 0.5f, 0.31f);`，而在图形学中通过点乘来确定最后物体反射的颜色的



```
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);

```

而我们的整个光照讲的都是如何在片段着色器中细分物体和光照的颜色，先总体理解颜色的最终输出是`lightColor` 点乘 `objectColor`的结果这一点非常重要，建议好好理解下这句话,这是整个光照的理论基础。


先来一个简单的[demo](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/07-%E5%85%89%E7%85%A7/%E9%A2%9C%E8%89%B2/Colors.cpp)

`colors.fs`

```
#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    FragColor = vec4(lightColor * objectColor,1.0);
}

```

![](https://user-images.githubusercontent.com/16829768/58758674-1274cb00-8551-11e9-9b0e-80934c1dee31.jpg)

接下来修改光源的颜色为绿色看下效果


```
glm::vec3 lightColor = glm::vec3(0.0f, 1.0f, 0.0f);
```

结果如下

![](https://user-images.githubusercontent.com/16829768/58758676-186aac00-8551-11e9-8a7d-061f17a4f311.jpg)

请自行调整demo查看不同的效果



## 二、冯氏光照模型

### 2.1 基本光照

嗯，请先阅读[这个](https://learnopengl-cn.github.io/02%20Lighting/02%20Basic%20Lighting/#_6)

看上面的例子会发现有个问题，无论我们怎么转动物体那个方向的物体颜色都是一样的，这样很明显不真实。为了让场景看起来更加真实，`冯氏光照模型`就出来了，冯氏光照模型的主要结构由3个分量组成：`环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。`


我们先看下下面这个场景，

![](https://user-images.githubusercontent.com/16829768/58758677-186aac00-8551-11e9-9de4-96f20046b7a0.jpg)

`DE`所在的平面表示物体的一个表面，从图上看`D`点离光源更近，按照我们的认知应该`D`比`E`要更加亮，那么如何用代码来描述这种情况呢，引入一个垂直于`DE`平面的向量，然后看夹角`A`和`B`,夹角越大说明里光源越远，那么所受的光的影响越小，这个正好对应的我们2个向量的点乘，当2个向量都是单位向量的时候夹角的余弦值和角度成反比，这个以后不再赘述了，而冯氏光照的`漫反射`说的就是这个

看下对比照
![](https://user-images.githubusercontent.com/16829768/58758681-26b8c800-8551-11e9-90c7-1fe4f798ed66.jpg)



再来一个场景

![](https://user-images.githubusercontent.com/16829768/58758680-26203180-8551-11e9-8db4-33a442a2da73.jpg)



图中的黄色向量是黑色的反射向量，对于同一个点，我们观察的点离反射向量的夹角越近应该影响越大，这个正好也可以用点乘来解决，冯氏光照的`镜面(Specular)光照`说的就是这个,来看下`ambient` + `specular`的样子

代码


```
    // 1.环境光照
    float ambientStrength = 0.1;
    vec3 ambient  = ambientStrength * lightColor;
    

    // 3.镜面光
    float specularStrength = 1.0;
    vec3 reflectDir = reflect(-lightDirection,norm);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),256);
    vec3 specular = specularStrength  * spec * lightColor;
    
    FragColor = vec4((ambient  + specular) * objectColor,1.0);
```

效果如下：

![](https://user-images.githubusercontent.com/16829768/58758682-26b8c800-8551-11e9-85af-7ad62da2c017.jpg)



完整的`冯氏光照`就是`ambient + diffuse + specular`，

完整的片段着色器代码


```
    // 1.环境光照
    float ambientStrength = 0.1;
    vec3 ambient  = ambientStrength * lightColor;
    
    // 2.漫反射
    vec3 lightDirection = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff =  max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff * lightColor;

    // 3.镜面光
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDirection,norm);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrength  * spec * lightColor;
    
    FragColor = vec4((ambient + diffuse + specular) * objectColor,1.0);
```

效果如下

![](https://user-images.githubusercontent.com/16829768/58758683-26b8c800-8551-11e9-9d87-f2920f0d3112.jpg)


对比下之前的效果

其实最好呈现到人眼中的颜色是非常复杂的，冯氏光照模型就是对现实的近似描述，把最后的颜色输出 = `环境光颜色` + `漫反射颜色` + `镜面反射颜色`

[完整代码](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/07-%E5%85%89%E7%85%A7/01-%E5%86%AF%E6%B0%8F%E5%85%89%E7%85%A7/Phong_light.cpp)

上面写的demo都是在假定物体的颜色在任何分量上都是一样的，然而实际的物体表面是各种各样的，所以对应的颜色分量也会不同，为了更加逼真的显示，这次把物体的各个分量也考虑进去，称为物体的材质。

### 2.2 物体的材质

根据冯氏光照模型的描述，最后的颜色输出 = `环境光颜色` + `漫反射颜色` + `镜面反射颜色`

那么对于光照来说应该是下面的数据结构

#### 2.2.1 光照模型

```
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

```

1. `position`:这个默认是点光源，所以有个位置属性；
2. `ambient`:就是在环境光分量的时候的影响，通常会设置为一个比较低的强度，因为我们不希望环境光颜色太过显眼；
3. `diffuse`:光源的漫反射分量通常设置为光所具有的颜色
4. `specular`:镜面光分量通常会保持为vec3(1.0)，以最大强度发光

#### 2.2.2 材质模型


```
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
```

1. `ambient`: 这个表示在环境光下应该是什么颜色，所以通常是物体本身的颜色
2. `diffuse`: 向量定义了在漫反射光照下物体的颜色，通常也是和物体颜色相同的颜色
3. `specular`: 物体镜面反射的颜色，一般设置为中等亮度的颜色；
4. `shininess`: 物体的反光度，我们不希望镜面分量在这个物体上过于强烈，一般设置为32。


#### 2.2.3 各个分量的颜色

可以得出下面的写法

```
vec3 ambient  = light.ambient * material.ambient;
vec3 diffuse  = diff * light.diffuse * material.diffuse;
vec3 specular = spec * light.specular * material.specular;

```

1. `diff`: 漫反射影响的强度；
2. `specular`:镜面反射影响的强度


而最后的颜色就是三者之和

```
vec3 result = ambient + diffuse + specular;
```

#### 2.2.4 一个简单的例子


```
void main() {
    // 1.环境光照
    vec3 ambient  = light.ambient * material.ambient;
    
    // 2.漫反射
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff =  max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // 3.镜面光
    vec3 reflectDir = reflect(-lightDirection,norm);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
    vec3 specular =   spec * light.specular * material.specular;
    
    FragColor = vec4((ambient + diffuse + specular),1.0);
}
```



设置

```
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
   
objectShader.setVec3("light.position", lightPos);
objectShader.setVec3("light.ambient",  lightColor * glm::vec3(0.2f));
objectShader.setVec3("light.diffuse",  lightColor * glm::vec3(0.5f));
objectShader.setVec3("light.specular", glm::vec3(1.0f));
   
objectShader.setVec3("material.ambient",  objectColor);
objectShader.setVec3("material.diffuse",  objectColor);
objectShader.setVec3("material.specular", glm::vec3(0.5f));
objectShader.setFloat("material.shininess", 32.0f);
```

效果

![](https://user-images.githubusercontent.com/16829768/58758684-27515e80-8551-11e9-8743-e4d654e519cf.jpg)


改变代码让物体的颜色随着光照的颜色改变而改变，效果自己运行看看吧


```
//        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor;
lightColor.x = sin(glfwGetTime() * 2.0f);
lightColor.y = sin(glfwGetTime() * 0.7f);
lightColor.z = sin(glfwGetTime() * 1.3f);
```

[地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/07-%E5%85%89%E7%85%A7/02-%E7%89%A9%E4%BD%93%E7%9A%84%E6%9D%90%E8%B4%A8/Material.cpp)

### 2.3 物体的贴图

前面我们使用的例子物体出来的颜色每个面都是一样的，但现实中的物体都是五彩缤纷的，之前的设置都是全局设置，不能对一个物体的视觉输出提供足够多的灵活性。

现在的问题就是如何对物体的每个像素设置不同的颜色，在纹理那张中说到，纹理就是数据的载体，我们可以将物体的颜色值写入到纹理中，然后通过纹理来逐个片段设置，在光照中称为`漫反射贴图`

#### 漫反射贴图

其实漫反射做的事情很简单，就是把之前统一设置物体的颜色改成了从纹理中读取数据


```
void main() {
    // 1.环境光照
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse,TexCoords));
    
    // 2.漫反射
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff =  max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,TexCoords));

    ...
}

```


其他的都是如何设置`material.diffuse`的代码

效果如下





![](https://user-images.githubusercontent.com/16829768/58758686-27e9f500-8551-11e9-88b7-7821bb830e65.jpg)


#### 镜面光贴图

上面也提到了，虽然比之前的纯色的立方体好看多了，但是木头哪一快太亮了，木头和周边的框不一样有一样的反光度，要想更精细的控制，也是要引入纹理，因为纹理可以控制每一个像素的显示，从而有了`镜面光贴图`。



其实就是把`material.specular`换成纹理


```
void main() {
   ...
    // 3.镜面光
    vec3 reflectDir = reflect(-lightDirection,norm);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
    vec3 specular =   spec * light.specular * vec3(texture(material.specular,TexCoords));
    
    FragColor = vec4((ambient + diffuse + specular),1.0);
}

```

效果如下


![](https://user-images.githubusercontent.com/16829768/58758687-27e9f500-8551-11e9-967a-189596901155.jpg)


可以看到确实真实了很多。

### 2.4 光源的类型


生活中光源大致分为以下三类：

1. 平行光，也叫定向光(DirectionalLight);
2. 点光源（Point Light）;
3. 聚光(Spotlight)


我们讨论的一切都是为了让我们的场景更加丰富，从而更加真实，下面我们看下对于同一个场景，不同类型的光源会产生什么样的视觉影响。

#### 平行光

就是将之前的点光源去掉


修改如下

平行光只有方向


```
struct Light {
    //vec3 position;
    vec3 direction;
};
```

```
// vec3 lightDirection = normalize(light.position - FragPos);
vec3 lightDirection = normalize(-light.direction);
```

效果如下,红色是坐标轴，黄色是光的方向



![](https://user-images.githubusercontent.com/16829768/58758688-28828b80-8551-11e9-940f-2de772851bd6.jpg)


#### 点光源


我们之前的都是用的点光源，但是一直都有一个问题，就是所有的箱子都是享受统一的光照强度，如下

![](https://user-images.githubusercontent.com/16829768/58758689-28828b80-8551-11e9-82d8-37b345af894e.jpg)


实际的场景对于点光源，应该距离点光源越远光线应该越弱，而`衰减`就是来说明光线传播距离的增长逐渐削的。


对于衰减已经有大师帮我们解决了，按照下面的公式就可以实现衰减了


![](https://user-images.githubusercontent.com/16829768/58758690-291b2200-8551-11e9-8c6e-3e5eda2e7f6b.jpg)


> 在这里d代表了片段距光源的距离。接下来为了计算衰减值，我们定义3个（可配置的）项：常数项Kc、一次项Kl和二次项Kq。

> 常数项通常保持为1.0，它的主要作用是保证分母永远不会比1小，否则的话在某些距离上它反而会增加强度，这肯定不是我们想要的效果。

> 一次项会与距离值相乘，以线性的方式减少强度。
>
> 二次项会与距离的平方相乘，让光源以二次递减的方式减少强度。二次项在距离比较小的时候影响会比一次项小很多，但当距离值比较大的时候它就会比一次项更大了。

这个是有参考值的，更加详细的说明请看[这](https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/#_4)

我们来实现下点光源的衰减


```
struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant; // 衰减函数的常数项
    float linear; // 衰减函数的一次项
    float quadratic; // 衰减函数的二次项
};

```

将最后的计算结果乘以衰减值

```
void main() {

    // 衰减值
    float distance = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
    
    vec3 result = ambient + diffuse + specular;
    
    result  *= attenuation;
    
    FragColor = vec4(result ,1.0);
}
```


结果如下


![](https://user-images.githubusercontent.com/16829768/58758691-291b2200-8551-11e9-98b4-917955d707e1.jpg)



可以看到离光源越近的越亮。

#### 聚光

可以将聚光想象成一个手电筒,它实际上是一个限定范围的点光源，有个照明的光圈，聚光的模型如下

![](https://learnopengl-cn.github.io/img/02/05/light_casters_spotlight_angles.png)


* LightDir：从片段指向光源的向量。
* SpotDir：聚光所指向的方向。
* Phiϕ：指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
* Thetaθ：LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小。

思路如下：

1. 计算出`LightDir`；
2. 计算`SpotDir`，一般就是摄像机的方向
3. 计算出`Thetaθ`


聚光的数据结构就是点光源的基础上添加一个`SpotDir`和切光角`ϕ`,


```
struct SpotLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant; // 衰减函数的常数项
    float linear; // 衰减函数的一次项
    float quadratic; // 衰减函数的二次项
    
    vec3 spotDir;
    float cutOff;
};
```
为了演示聚光，我们这次把光源的位置跟随摄像机的位置，能保证我们的观看方向一点是正对光源的，

```
objectShader.setVec3("light.position", camera.Position);
```



```
 vec3 lightDirection = normalize(light.position - FragPos);
     // 计算Thetaθ角的余弦值
    float theta = dot(lightDirection, normalize(-light.spotDir));
    
    if (theta < light.cutOff) {// 比较的余弦值的大小，余弦值越大角度越小，
        FragColor = vec4 (light.ambient * vec3(texture(material.diffuse,TexCoords)) ,1.0f);
    } else {
       // 1.环境光照
        vec3 ambient  = light.ambient * vec3(texture(material.diffuse,TexCoords));

        // 2.漫反射
        vec3 norm = normalize(Normal);
        float diff =  max(dot(norm,lightDirection),0.0);
        vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,TexCoords));

        // 3.镜面光
        vec3 reflectDir = reflect(-lightDirection,norm);
        vec3 viewDir = normalize(cameraPos - FragPos);
        float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
        vec3 specular =   spec * light.specular * vec3(texture(material.specular,TexCoords));

        // 衰减值
        float distance = length(light.position - FragPos);
        float attenuation = 1 / (light.constant + light.linear * distance +
                                 light.quadratic * (distance * distance));

        vec3 result = ambient + diffuse + specular;

        result  *= attenuation;
      FragColor = vec4(result  ,1.0);
    }
```

可以看到聚光本质上还是点光源，只是在点光源的点亮范围画了一个圈,而且在圈内的影响是一样的，如下

![](https://user-images.githubusercontent.com/16829768/58758692-291b2200-8551-11e9-8f5a-b6c2a7fa42a5.jpg)


是不是有点想狙击手的瞄准器? [代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/07-%E5%85%89%E7%85%A7/04-%E5%85%89%E6%BA%90/SpotLight.cpp)

##### 补充

如何你不想有个明显的边缘的话，就要利用`theta`值，可以使用下面的公式

![](https://user-images.githubusercontent.com/16829768/58758694-29b3b880-8551-11e9-98de-f5e6ccc6a0de.jpg)

这里ϵ(Epsilon)是内（ϕ）和外圆锥（γ）之间的余弦值差（ϵ=ϕ−γ）。最终的I值就是在当前片段聚光的强度。详情[参考](https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/#_9)

画个图理解下这个公式


![](https://user-images.githubusercontent.com/16829768/58758695-29b3b880-8551-11e9-8f05-26e4c1586f96.jpg)

证明如下


![](https://user-images.githubusercontent.com/16829768/58758661-f113df00-8550-11e9-8801-a1e2f8463a28.png)
我们现在有了一个在聚光外是负的，在内圆锥内大于1.0的，在边缘处于两者之间的强度值了。如果我们正确地约束(Clamp)这个值，在片段着色器中就不再需要if-else了，我们能够使用计算出来的强度值直接乘以光照分量：


```
...
// 计算Thetaθ角的余弦值
float theta = dot(lightDirection, normalize(-light.spotDir));
float epsilon = light.cutOff -  light.outerCutOff;
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //clamp 将结果限制在0-1

// 将不对环境光做出影响，让它总是能有一点光
diffuse  *= intensity;
specular *= intensity;
    
FragColor = vec4(result  ,1.0);    
  
```

### 2.5 多个光源

这个就是说场景中有很多光源该如何处理，这个很简单，可以自己尝试写一下，可以参考[这个](https://learnopengl-cn.github.io/02%20Lighting/06%20Multiple%20lights/)

