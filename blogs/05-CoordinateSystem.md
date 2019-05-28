# 坐标系统

请先阅读[坐标系统](https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/#_5)

   * [坐标系统](#坐标系统)
      * [为什么要用4阶矩阵](#为什么要用4阶矩阵)
      * [W分量的作用](#w分量的作用)
      * [不要太拘泥于这些矩阵了，比如perspective、ortho，这些都是写快捷写法](#不要太拘泥于这些矩阵了比如perspectiveortho这些都是写快捷写法)
      * [注意矩阵运算的顺序](#注意矩阵运算的顺序)
      * [顶点着色器之后会自动执行透视除法](#顶点着色器之后会自动执行透视除法)
      

## 为什么要用4阶矩阵

细心的读者应该发现了在`coordinate.vs`中我们传入的都是`mat4`四分量的

```
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
```

这是因为旋转和缩放的转换矩阵只需要3列，但是为了处理平移，至少需要4列矩阵

$ \begin{bmatrix}  \color{red}1 & \color{red}0 & \color{red}0 & \color{red}{T_x} \\ \color{green}0 & \color{green}1 & \color{green}0 & \color{green}{T_y} \\ \color{blue}0 & \color{blue}0 & \color{blue}1 & \color{blue}{T_z} \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} x + \color{red}{T_x} \\ y + \color{green}{T_y} \\ z + \color{blue}{T_z} \\ 1 \end{pmatrix} $


为了要实现平移左边需要4X4的矩阵，然而，4列矩阵不能与3维向量相乘，只能与4维向量相乘，这就是为什么我们使用齐次的4维向量取代3维向量。




## W分量的作用

为了让我们的场景看起来更真实，比如离观察者越近的物体应该看起来更大，越远的物体应该看起来更小，这样场景看清来会更真实，从而引入了透视投影这个概念，比如下面的这个矩形，当离观察者的距离为1时是下面这个样子



![](https://user-images.githubusercontent.com/16829768/58414859-3d71a180-80af-11e9-9025-3d9bfe273dfa.png)

当离观察者的距离为2时就变成了虚线的样子

![](https://user-images.githubusercontent.com/16829768/58414901-5ed28d80-80af-11e9-82e7-6646d9d3277d.png)

这样才会更真实,而透视投影矩阵就是将图1变成图2的矩阵

那么透视投影矩阵是如何影响我们的场景的呢，其实它是通过位置的`z`值来间接影响的

我们可以知道的一点是，如果以观察者为参照物，那么离观察者越远，`z`值越大，看下面的例子

![](http://latex.codecogs.com/gif.latex?\begin{bmatrix}%201%20&%200%20&%200%20&%200%20\\%200%20&%201%20&%200%20&%200%20\\%200%20&%200%20&%201%20&%200%20\\%200%20&%200%20&%201%20&%200%20\end{bmatrix}%20\begin{bmatrix}%202%20\\%203%20\\%204%20\\%201%20\end{bmatrix}%20=%20\begin{bmatrix}%202%20\\%203%20\\%204%20\\%204%20\end{bmatrix})

可以看到通过左边的透视矩阵，将点的`z`值来影响`w`分量的，然后通过除以`w`分量，下面展示了什么是透视除法

$\frac{1}{4} (2, 3, 4, 4) = (0.5, 0.75, 1, 1)$

以上面的例子那么就是(4,4) 变成了 (2,2)...，所以图片小了一半


比如我们的场景


![](https://user-images.githubusercontent.com/16829768/58414909-64c86e80-80af-11e9-80e8-c17f336ea646.png)

就有以下的特性:

* 稍微向后倾斜至地板方向。
* 离我们有一些距离。
* 有透视效果（顶点越远，变得越小）。

如果不使用透视投影矩阵就是下面这个样子，自己感受下



![](https://user-images.githubusercontent.com/16829768/58414913-685bf580-80af-11e9-9d1f-bd13abd13782.png)
再看这张图

![](https://learnopengl-cn.github.io/img/01/08/coordinate_systems_multiple_objects.png)

其实我们只是做了平移并没有缩放操作，它为什么为看起来大小不一呢，这也是透视投影矩阵左右的结果，因为它们的`z`不一样



## 不要太拘泥于这些矩阵了，比如`perspective`、`ortho`，这些都是写快捷写法

以`perspective`为例


```
 projection = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
```

本质上就是一个矩阵数据，可以断点调试

![](https://user-images.githubusercontent.com/16829768/58414715-d48a2980-80ae-11e9-9afc-fd92905bd6e7.png)


glm:是列向量，所以写成矩阵的样子就是下面这样


$\begin{bmatrix}  1.58 & 0 & 0 & 0 \\  0 & 2.41 & 0 & 0 \\  0 & 0 & -1.002 & -0.2 \\  0 & 0 & -1 & 0  \end{bmatrix} $

如果你能构造出也是可以的，读者可以自行尝试，后面我们还会遇到很多这样的便捷矩阵。


## 注意矩阵运算的顺序

$ V_{clip} = M_{projection} \cdot M_{view} \cdot M_{model} \cdot V_{local}$

## 顶点着色器之后会自动执行透视除法

[demo地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/05-%e5%9d%90%e6%a0%87%e7%b3%bb%e7%bb%9f/CoordinateSystem.cpp)


