# 数学基础


   * [数学基础](#数学基础)
      * [【重点1】：向量相减](#重点1向量相减)
         * [如果2个向量相减，那么相减的结果的箭头指向被减向量。](#如果2个向量相减那么相减的结果的箭头指向被减向量)
      * [【重点2】：点乘](#重点2点乘)
      * [【重点3】：叉乘](#重点3叉乘)
      * [【重点4】：矩阵相乘是不遵守交换律的](#重点4矩阵相乘是不遵守交换律的)


请务必先阅读:

[变换](https://learnopengl-cn.github.io/01%20Getting%20started/07%20Transformations/#_1)

接下来画重点

##  【重点1】：向量相减

### 如果2个向量相减，那么相减的结果的箭头指向被减向量。









![image](https://user-images.githubusercontent.com/16829768/58300048-b654cd00-7e13-11e9-8c06-3fb5bb7842c5.png)

例如上图 $\vec{AB}$ = $\vec{AC}$ + $\vec{CB}$,

那么 $\vec{AB}$ - $\vec{AC}$ = $\vec{CB}$ 最终箭头是指向`B`的

通过平移得到$\vec{DB}$ 等于$\vec{AC}$



![image](https://user-images.githubusercontent.com/16829768/58300075-d4bac880-7e13-11e9-8dac-815d2898a837.png)

那么 $\vec{AB}$ - $\vec{CB}$ = $\vec{AC}$ = $\vec{DB}$ 最终箭头也是指向`B`的

在三维空间中，有的时候无法明确表示出坐标，通过这个可以判断出，最后向量执行运算后的方向。

##  【重点2】：点乘


$\vec{v}$ ${\cdot }$ $\vec{k}$ = |$\vec{v}$| ${\cdot }$ | $\vec{k}$| cos${\theta}$

如果将$\vec{v}$ 和 $\vec{k}$变成单位向量，那么点乘的结果就是直观的反应2个向量的夹角


$\vec{v}$ ${\cdot }$ $\vec{k}$ = 1 ${\cdot }$ 1 cos${\theta}$


![image](https://user-images.githubusercontent.com/16829768/58299851-0aab7d00-7e13-11e9-90c7-0188a3163b6f.png)

可以看出点乘的结果越大，说明2个向量方向越趋向一致，夹角越小。


##  【重点3】：叉乘

> 叉乘只在3D空间中有定义，它需要两个不平行向量作为输入，生成一个正交于两个输入向量的第三个向量。
> 如果输入的两个向量也是正交的，那么叉乘之后将会产生3个互相正交的向量。接下来的教程中这会非常有用


## 【重点4】：矩阵相乘是不遵守交换律的

矩阵相乘是满足交换律的，所以顺序很重要，矩阵相乘时，在最右边的矩阵是第一个与向量相乘的，所以你应该从右向左读这个乘法。建议在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，否则它们会（消极地）互相影响



通过矩阵变换将图像逆时针旋转90度：

[代码地址](https://github.com/fangshufeng/LearnOpenGL/blob/master/Glitter/Sources/04-%E5%8F%98%E6%8D%A2/Transform.cpp)

通过矩阵变换绕着原点随时间旋转,修改如下


```
//    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f,0.0f,1.0f));
//    shader.setMat4("transform", transform);
    
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
    shader.setMat4("transform", transform);
```

