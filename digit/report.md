# 手写数字识别 实验报告

马子轩 计研22 2019211119

### 一、实验要求

1. Kaggle账号注册

2. 实现基于MNIST数据集的手写数字识别

3. 实现方法: 本实验基于pyTorch实现了MLP和CNN两种方式

### 二、任务分析

本次实验是一次深度神经网络的实践, 基于MNIST公开数据集, 进行手写数字的识别. 

首先, 我们需要针对输入训练集进行预处理.
我们将训练数据分为两部分, 其中40000条作为训练集, 2000条作为验证集.

训练部分我们分别实现了MLP和LeNet-5两种网络结构.

### 三、模型分析

#### 1. MLP

我们的MLP实现网络结构如下图所示.

MLP(

  (fc1): Linear(in_features=784, out_features=512, bias=True)

  (fc2): Linear(in_features=512, out_features=512, bias=True)

  (fc3): Linear(in_features=512, out_features=10, bias=True)

  (droput): Dropout(p=0.2, inplace=False)

)

其中, fc1和fc2后均有Relu层.

损失函数使用交叉熵, 优化器使用SGD.

#### 2. LeNet-5

我们实现的CNN网络结构如下所示

LeNet5(

  (conv1): Conv2d(1, 6, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2))
  
  (conv2): Conv2d(6, 16, kernel_size=(5, 5), stride=(1, 1))
  
  (fc1): Linear(in_features=400, out_features=120, bias=True)
  
  (fc2): Linear(in_features=120, out_features=84, bias=True)
  
  (fc3): Linear(in_features=84, out_features=10, bias=True)

)

其中, conv1和conv2后均有pooling层. fc1和fc2后有relu层.

损失函数使用交叉熵, 优化器使用Adam.

### 四、实现过程

#### 1. Kaggle

id: mazxhpc

![fig1](digit_result.png)

两个模型均只提交1次.

#### 2. MLP

我们选择batchsize=256, learning rate = 0.01, 训练24个epoch后收敛.

训练128个epoch后, 准确度达到97.3%

#### 3. CNN

我们选择batchsize=256, learning rate = 0.01, 训练6个epoch后收敛.

训练128个epoch后, 准确度达到98.5%

#### 4. 实验平台

实验平台:

CPU: duel Intel(R) Xeon(R) CPU E5-2680 v4 @ 2.40GHz
memory: 256GB
GPU: Nvidia tesla V100 32GB

OS: ubuntu 16.04
Torch: 1.7.0

#### 5. 实现流程

我们基于pyTorch实现了两种网络的训练.
首先是网络的定义, torch中网络的定义较为容易.直接将层的描述输入即可.

train: 每个batch分别运行forward, 计算loss, 计算backward.

valid: 执行forward, 比较执行结果.

test: 与valid一致.

#### 6. 目录组织

```
    train.csv
    test.csv
    lenet-5.py
    mlp.py
```

#### 7. 执行方法

CNN

``` shell
python lenet-5.py
```

MLP

``` shell
python mlp.py
```

### 五、结果分析

模型的参数选择, 会对实验结果造成一些影响.

例如batchsize的选择, 目前版本选择为256, 使用64时, MLP不能收敛, CNN使用更多个epoch才能够收敛.
learning rate也会影响收敛程度, 例如0.001时收敛使用了更多的epoch.
网络的结构也有所影响, 例如MLP的第二个hidden layer改成128, 模型精度就会下降到约95%. 这也是模型参数比较小造成的结果.

### 六、问题思考

#### 1. 实验训练什么时候停止是最合适的？简要陈述你的实现方式，并试分析固定迭代次数与通过验证集调整等方法的优缺点。

实际使用时, 我们经常固定epoch个数, 因为训练过程中我们会打checkpoint, 如果收敛, 可以及时停止, 并使用合适的版本. 如果没有收敛, 也可以从checkpoint继续, 也更灵活可控. 而通过验证集调整有时候由于模型因素可能达不到固定要求. 这种方式就不是很合适了.

#### 2. 实验参数的初始化是怎么做的？不同的方法适合哪些地方？（现有的初始化方法为零均值初始化，高斯分布初始化，正交初始化等）

我们在实验中使用了正态分布做初始化. 这几种初始化方法在不同问题中有着不一样的表现. 这个内容不能进行一概而论的分析. 

#### 3. 过拟合是深度学习常见的问题，有什么方法可以方式训练过程陷入过拟合

过拟合确实是常见的问题, 针对MNIST这种规模比较小的问题, 最合适的方法是选择适当的网络规模, 例如MLP中不使用过大过多的hidden layer. 而较小的模型可能能力不够, 这种情况下选择合适的网络需要进行trade-off.

另外还有一些例如使用dropout等技术, 或是使用合适的激活函数, 加入正则项等, 都能够一定程度的防止过拟合问题, 但是作用机制并不是一个很明确的解释.

#### 4. 试分析 CNN（卷积神经网络）相对于全连接神经网络的优点

CNN通过部分连接, 降低了大量的参数. 在最后的全连接层中, 连接可以共享参数, 降低了参数.
Pooling层, 降低参数. 总体来说, CNN在保持模型能力的情况下, 通过对参数进行降低, 提高模型的精度, 提高性能.

### 七、总结

本次实验中, 我们对MNIST数据集上的手写数字识别进行了实践. 实现了MLP和CNN两类网络. 并对其中部分参数进行了枚举实验, 在这类图像识别问题中, CNN确实是一个比较合适的选择.
