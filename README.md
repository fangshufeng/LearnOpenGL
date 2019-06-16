# LearnOpenGL


   * [LearnOpenGL](#learnopengl)
      * [使用](#使用)
         * [安装一些子依赖](#安装一些子依赖)
         * [生成xcode工程](#生成xcode工程)
      * [笔记](#笔记)
      * [感谢](#感谢)

      
## 使用

### 安装一些子依赖


```
git clone https://github.com/fangshufeng/LearnOpenGL.git

```

目录如下


```
|-blogs
|-CMakeLists.txt
|-Glitter
|-README.md
|-Samples
```

```sh


cd Glitter
mkdir Vendor
cd Vendor

git clone  https://github.com/assimp/assimp.git
git clone -b c https://github.com/Dav1dde/glad.git
git clone  https://github.com/glfw/glfw.git
git clone  https://github.com/g-truc/glm.git
git clone  https://github.com/nothings/stb.git
git clone  https://github.com/bulletphysics/bullet3.git bullet
```


### 生成xcode工程

回到项目目录

```
cd ../..
```

执行下面的命令

```
mkdir Build
cd Build
cmake -G "Xcode" ../.

```

添加`Source`文件夹,打开任意你想运行的`.cpp`文件，比如`Texture_filtering.cpp`

![image](https://user-images.githubusercontent.com/16829768/59560153-e58be200-903f-11e9-9547-b4920beb92f3.png)



运行结果

![](https://user-images.githubusercontent.com/16829768/58221260-7a9f0200-7d44-11e9-8db6-fed26a3a6584.png)


## 笔记

记录自己学习过程中的一些心得，可以见`blogs`目录下


* [00.LearnOpenGL-基本库介绍](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/00.LearnOpenGL-%E5%9F%BA%E6%9C%AC%E5%BA%93%E4%BB%8B%E7%BB%8D.md)
* [01.HelloWorld](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/01.HelloWorld.md)
* [02.DrawPrimitive](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/02.DrawPrimitive.md)
* [03.Texture](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/03.Texture.md)
* [04-transform](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/04-transform.md)
* [05-CoordinateSystem](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/05-CoordinateSystem.md)
* [06-Light](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/06-Light.md)
* [07-测试与混合](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/07-TestAndBlend.md)
* [08-帧缓冲与后期处理](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/08-framebuffers.md)

## 感谢

1. [Glitter](https://github.com/Polytonic/Glitter)
2. [LearnOpenGL CN](https://learnopengl-cn.github.io/01%20Getting%20started/01%20OpenGL/)



