# 模型加载

   * [模型加载](#模型加载)
      * [准备](#准备)
      * [3者的关系](#3者的关系)
         * [简单认识nanosuit.obj文件格式](#简单认识nanosuitobj文件格式)
         * [scene数据如下](#scene数据如下)
         * [查看mChildren的数据](#查看mchildren的数据)
         * [查看mMesh的数据](#查看mmesh的数据)
         * [查看mVertices的数据](#查看mvertices的数据)

## 准备

1. [Assimp](https://learnopengl-cn.github.io/03%20Model%20Loading/01%20Assimp/)
2. [Mesh](https://learnopengl-cn.github.io/03%20Model%20Loading/02%20Mesh/)
3. [Model](https://learnopengl-cn.github.io/03%20Model%20Loading/03%20Model/)

## 3者的关系

由于不同的文件格式有它自己的模型数据，需要对不同的模型格式进行不同的解析，`Assimp`库帮我们把这些模型格式抽象为一个特点的数据结构方便我们使用格式如下

![](https://learnopengl-cn.github.io/img/03/01/assimp_structure.png)


通过文件`nanosuit.obj`来解释下这个图

###  简单认识`nanosuit.obj`文件格式

```cpp
# Blender v2.64 (sub 0) OBJ File: 'nanosuit.blend'
# www.blender.org
mtllib nanosuit.mtl


o Visor
v 0.320384 14.057541 0.507779
v 0.385196 13.984534 0.445066
v 0.416643 14.114325 0.462461
[...]

o Legs
v 1.899165 2.317634 -0.120600
v 1.903005 2.477843 -0.148509
v 1.899165 2.518403 -0.160526
v 1.970218 2.609661 -0.259889
[...]


o hands
v -3.554696 8.314335 1.450770
v -3.537413 8.306223 1.449854
v -3.512448 8.289999 1.458094
[...]

o Arms
v 3.658015 9.133632 1.108368
v 3.652269 9.133632 1.125763
v 3.667592 9.190415 1.074494
[...]

o Helmet
v 0.169514 13.749290 0.552182
v 0.214883 13.879080 0.523343
v 0.182237 13.911528 0.565000
[...]

o Body
v 1.413312 11.769991 -1.458770
v 1.418113 11.761879 -1.422149
v 1.420994 11.607754 -1.503631
[...]

o Lights
v 0.519383 14.098101 0.399747
v 0.474254 14.073765 0.435453

```

意思这个模型有`7`个局部。

通过`Assimp`库的` importer.ReadFile`方法


```cpp
 void loadModel(string const &path) {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));
        
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }
    
```

###  `scene`数据如下

```cpp
(lldb) p *scene
(const aiScene) $19 = {
  mFlags = 0
  mRootNode = 0x00000001022f0c00
  mNumMeshes = 7
  mMeshes = 0x0000000101cdede0
  mNumMaterials = 7
  mMaterials = 0x0000000101cdee20
  mNumAnimations = 0
  mAnimations = 0x0000000000000000
  mNumTextures = 0
  mTextures = 0x0000000000000000
  mNumLights = 0
  mLights = 0x0000000000000000
  mNumCameras = 0
  mCameras = 0x0000000000000000
  mMetaData = 0x0000000000000000
  mPrivate = 0x0000000101c3ec40
}
```

1. `mRootNode`也就是上图中的`mRootNode`，地址为`0x00000001022f0c00`
2. `mMeshes`这里存放着我们场景中的所有`mesh`（可以理解为组成身体各个部分）
3. `mMaterials`这里存放着所以的纹理数据


现在要做的事情就是把存放在`scene`里面的数据转为我们的`Mesh`数据结构

下面来看下`mRootNode`


```cpp
(lldb) p *(aiNode *)0x00000001022f0c00
(aiNode) $20 = {
  mName = (length = 12, data = char [1024] @ 0x00007ffb4d55cc08)
  mTransformation = (a1 = 1, a2 = 0, a3 = 0, a4 = 0, b1 = 0, b2 = 1, b3 = 0, b4 = 0, c1 = 0, c2 = 0, c3 = 1, c4 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 1)
  mParent = 0x0000000000000000
  mNumChildren = 7
  mChildren = 0x0000000101d8c7e0
  mNumMeshes = 0
  mMeshes = 0x0000000000000000
  mMetaData = 0x0000000000000000
}
```

 1. 由于`mNumMeshes == 0`，所以`mRootNode`没有`Mesh`,按照上图需要查看`children`

### 查看`mChildren`的数据


![](https://user-images.githubusercontent.com/16829768/60154499-12977c00-981a-11e9-91ac-042a89344197.jpg)


1. 可以看出为我们的`Visor`实体在`mMesh[]`的索引值为`0`；
2. `Legs`实体在`mMesh[]`的索引值为`1`；



先看自己的`mesh`索引，没有再看子节点的。

### 查看`mMesh[]`的数据


![](https://user-images.githubusercontent.com/16829768/60154500-13301280-981a-11e9-87df-99fd2a368620.jpg)


确实在`mMesh[]`中的`0`和`1`处找到了数据


### 查看`mVertices[]`的数据

以`mMesh[1]`也就是`Legs`为例

```cpp
(lldb) p (aiVector3D *)0x0000000113d95000
(aiVector3D *) $51 = 0x0000000113d95000
(lldb) p *(aiVector3D *) $51 
(aiVector3D) $52 = (x = 1.89916492, y = 2.31763411, z = -0.1206)
(lldb) p *(aiVector3D *) ($51 + 1) 
(aiVector3D) $53 = (x = 1.903005, y = 2.47784305, z = -0.148508996)
(lldb) p *(aiVector3D *) ($51 + 2) 
(aiVector3D) $54 = (x = 1.89916492, y = 2.51840305, z = -0.160526007)
(lldb) 

```

可以和上面的`.obj`文件对比是一样的。


所以整个结构类似下面的


```
.
├── Material[] 存放场景中所有材质，数组
├── mMesh[]  存放着场景中所以的模型数据，
│   ├── mesh1 
│   │   ├── mTextureCoords 纹理坐标
│   │   ├── normal 法向量
│   │   ├── vertex 顶点
│   │   └── ...其他
│   ├── mesh2
│   │   ├── mTextureCoords 纹理坐标
│   │   ├── normal 法向量
│   │   ├── vertex 顶点
│   │   └── ...其他
│   └── ... 其他的mesh
└── mRootNode
    ├── mChildren[]
    └── mMesh[]

```


所以在将`aiMesh`转成`Mesh`的写法会是下面这样


```cpp
void processNode(aiNode *node, const aiScene *scene)
{
   // process each mesh located at the current node
   for(unsigned int i = 0; i < node->mNumMeshes; i++)
   {
       // the node object only contains indices to index the actual objects in the scene.
       // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
       aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       meshes.push_back(processMesh(mesh, scene));
   }
   // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
   for(unsigned int i = 0; i < node->mNumChildren; i++)
   {
       processNode(node->mChildren[i], scene);
   }
   
}
```



那么，三者的关系很简单，`Model`就是讲`asMesh`转变成`Mesh`的中介者。

