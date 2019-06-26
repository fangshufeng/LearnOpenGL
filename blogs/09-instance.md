# 实例化
   * [实例化](#实例化)
      * [一、预习](#一预习)
      * [二、为什么要实例化](#二为什么要实例化)
      * [三、为什么要实例数组](#三为什么要实例数组)
      * [四、行星demo](#四行星demo)

      
## 一、预习

 1. [实例化](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/10%20Instancing/)
 
  
## 二、为什么要实例化

由于`OpenGL`在绘制顶点数据之前需要做很多准备工作（比如告诉GPU该从哪个缓冲读取数据，从哪寻找顶点属性，而且这些都是在相对缓慢的CPU到GPU总线(CPU to GPU Bus)上进行的）。所以，即便渲染顶点非常快，命令GPU去渲染却未必

为了减少数据从CPU到GPU的传输，使用`glDrawArraysInstanced`或者`glDrawElementsInstanced`

比如要画下面的场景

![](https://learnopengl-cn.github.io/img/04/10/instancing_quads.png)

* 未使用实例化前会这样做的，[代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/15-%E5%AE%9E%E4%BE%8B%E5%8C%96/normal/NormalDraw.cpp)
* 实例化后会是这样，[代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/15-%E5%AE%9E%E4%BE%8B%E5%8C%96/instanceID/Instance.cpp)

## 三、为什么要实例数组

上面我们是通过在着色器中定义`Uniform`变量来传递数据的，但是不同的着色器的`Uniform`是有上限的，如果我们的数据超过了这个数量，那么数据将无法传递了，各个着色器的上面分别是下面的宏查看


```c

GL_MAX_VERTEX_UNIFORM_COMPONENTS// 顶点着色器 0x8B4A 
GL_MAX_GEOMETRY_UNIFORM_COMPONENTS // 几何着色器 0x8DDF
GL_MAX_FRAGMENT_UNIFORM_COMPONENTS  // 片段着色器 0x8B49

```

为了解决这个上限的问题，所以有了实例化数组这个东西，具体这个实例化数组是个什么东西，其实就是一个和`color`和`position`一样的缓冲，开辟一段内存然后标记该如何解析这段内存


```cpp
GLuint instanceVBO;
glGenBuffers(1,&instanceVBO);
glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
glBufferData(GL_ARRAY_BUFFER,sizeof(translations) ,translations,GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER,0);

glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
glEnableVertexAttribArray(2);
glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),(void *)0);
```

到这里都没什么新的东西，要想开启实例化数组，那么要使用下面的这个函数


```diff
+ glVertexAttribDivisor(2, 1);
```

第一个参数表示的是索引值，第二个表示的是没几个实例更新一次数据，搭配`glDrawArraysInstanced(GL_TRIANGLES,0,6,100);`就能画出`100`个矩形了，如果我们去掉`glVertexAttribDivisor`看看会绘制出什么呢


```diff
...
glGenBuffers(1,&VBO);
glBindBuffer(GL_ARRAY_BUFFER,VBO);
glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER,0);

GLuint instanceVBO;
glGenBuffers(1,&instanceVBO);
glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
glBufferData(GL_ARRAY_BUFFER,sizeof(translations) ,translations,GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER,0);

glGenVertexArrays(1,&VAO);
glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER,VBO);
glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
glEnableVertexAttribArray(1);

glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
glEnableVertexAttribArray(2);
glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),(void *)0);
- glVertexAttribDivisor(2, 1);
```



![](https://user-images.githubusercontent.com/16829768/60154412-d95f0c00-9819-11e9-8885-b04a7073190b.jpg)


造成这样的结果不是bug，是因为一旦我们不调用`glVertexAttribDivisor`，就会当做普通的属性处理，由于我们的场景是通过6个点画出2个三角形，正好是这6个点，大家可以自己算下这6个点的位置。

再修改代码如下


```diff
...
glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
glEnableVertexAttribArray(2);
glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),(void *)0);
- glVertexAttribDivisor(2, 1);
+ glVertexAttribDivisor(2, 2);
```

![](https://user-images.githubusercontent.com/16829768/60154381-bdf40100-9819-11e9-9b01-8ca5dbc991b2.png)

因为现在是没2个实例更新一个offset的。

## 四、行星demo


下面来一个具体的案例

![](https://user-images.githubusercontent.com/16829768/60154414-db28cf80-9819-11e9-814d-c555f79c6f79.jpg)

我们最终需要画一个上面的场景：

1. 我们需要2个模型文件，第一个大行星和一个小行星；
2. 先画中间的大行星；
3. 周边的小行星可以看出一个小行星然后位移得到

具体可以看
[此处](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/10%20Instancing/#_3)，
[代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/15-%E5%AE%9E%E4%BE%8B%E5%8C%96/instanceModel/InstanceModel.cpp)


