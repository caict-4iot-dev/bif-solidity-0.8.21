## 编译solidity

### 编译路径

172.17.6.84：/home/chenyuhuan/solidity

### 安装依赖

+ 安装boost1.77.0

下载地址：https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.zip

安装命令：

```bash
$ unzip boost_1_77_0.zip
$ cd boost_1_77_0
$ ./bootstrap.sh
$ ./b2 -j4 -d0 link=static runtime-link=static variant=release threading=multi address-model=64 --with-filesystem --with-system --with-regex --with-program_options --with-test --prefix=/home/chenyuhuan/boost install
```

+ gcc 8+ 、cmake 3.21.3+



### 编译solidity(需要连接外网)

```bash
$ mkdir build
$ cd build
$ cmake -DBoost_USE_STATIC_RUNTIME=ON -DBoost_DIR=/home/chenyuhuan/boost/lib/cmake/Boost-1.77.0 ..
$ make -j4
```



### 修改代码后编译 

编译要加-DPEDANTIC=OFF，否则一些类型转换警告会变成error

```bash
$ mkdir build
$ cd build
$ cmake -DPEDANTIC=OFF -DBoost_USE_STATIC_RUNTIME=ON -DBoost_DIR=/home/chenyuhuan/boost/lib/cmake/Boost-1.77.0 ..
$ make -j4
```

boost路径也可以不用指定到cmake

```
$ cmake -DPEDANTIC=OFF -DBoost_USE_STATIC_RUNTIME=ON -DBoost_DIR=/home/chenyuhuan/boost ..
```



## 编译sol合约

编译sol合约生成paycode

```bash
$ build/sol/sol --bin test.sol
```

在evmone单元测试中，部署调用合约   34机器    /home/share/cyh/bif-evmone/evmone



## 测试

执行build_test.py脚本，把openzeppelin中所有的合约编译一遍

```bash
$ python build_test.py
```



## git patch

生成补丁

```bash
$ git diff 5405e6742a0b86be089cce40cbc4951d8c652e3a > commit.patch
```

应用补丁

```bash
# 检查patch是否可以应用
git apply --check commit.patch
# 打单个补丁
git apply commit.patch
# 打多个补丁
git apply ../patch/*.patch
```


js编译器生成 一键部署
sh script/build_emscripten.sh

