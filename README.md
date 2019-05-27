# LearnOpenGL

## 使用

### 安装一些子依赖

```sh
cd Glitter/Vendor

git clone  https://github.com/assimp/assimp.git
git clone -b c https://github.com/Dav1dde/glad.git
git clone  https://github.com/glfw/glfw.git
git clone  https://github.com/g-truc/glm.git
git clone  https://github.com/nothings/stb.git
git clone  https://github.com/bulletphysics/bullet3.git bullet
```

### 生成xcode工程

回到项目目录,执行下面的命令

```
mkdir Build
cd Build
cmake -G "Xcode" ../.

```

## 目的

记录自己学习过程中的一些心得，可以见`blogs`目录下


* [00.LearnOpenGL-基本库介绍](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/00.LearnOpenGL-%E5%9F%BA%E6%9C%AC%E5%BA%93%E4%BB%8B%E7%BB%8D.md)
* [01.HelloWorld](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/01.HelloWorld.md)
* [02.DrawPrimitive](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/02.DrawPrimitive.md)
* [03.Texture](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/03.Texture.md)
* [04-transform](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/04-transform.md)
* [05-CoordinateSystem](https://github.com/fangshufeng/LearnOpenGL/blob/master/blogs/05-CoordinateSystem.md)

[感谢](https://github.com/Polytonic/Glitter)

