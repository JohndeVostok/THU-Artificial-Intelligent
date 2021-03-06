# 拼音输入法 实验报告

马子轩 计研22 2019211119

### 一、实验要求

1. 使用基于字的二、三、四元模型，与基于词的二元模型，实现拼音到汉字的转换

2. 介绍实现思路，解释实现过程

3. 展示实验结果，选取案例分析

4. 对比参数选择，进行性能分析

5. 总结收获经验，提出改进方案

### 二、任务分析

本次实验的主要任务是设计基于贝叶斯理论的输入法，即寻找符合该拼音的句子中概率最高的一个。

任务的输入包括input.txt，包括一系列的句子的拼音。
任务的输出包括这一系列拼音对应的短语。

其他需求的包括语料库，针对语料库进行分析构造分析用的数据。

总的来说，我们需要提供一个拼音串到汉语短语的转换过程。

### 三、模型分析

基于字的模型是基于贝叶斯理论设计的算法模型。

我们都知道贝叶斯公式: P(X|Y) = P(X)P(Y|X)/P(Y)

这里我们令X=汉语短语，Y=拼音句子

令X={c0,c1,c2,...,cn}，那么有，P(X)=P(c0,c1,c2,...,cn)
P(X) = P(c0)P(c1|c0)P(c2|c0,c1)P(c3|c0,c1,c2)...P(Cn|c0,c1,...,cn-1)

那么一元模型中，我们认为，每个字符的概率之和自己的概率相关；
二元模型中，每个字符只与前一个字符相关；三元中与前两个相关；四元模型与前三个相关。

基于此，在一元模型中:

P(X) = P(c0)P(c1)...P(cn)

二元模型中:

P(X) = P(c0)P(c1|c0)P(c2|c1)...P(cn|cn-1)

三元模型中:

P(X) = P(c0)P(c1|c0)P(c2|c0c1)...P(cn|cn-2cn-1)

四元模型中:

P(X) = P(c0)P(c1|c0)P(c2|c0c1)P(c3|c0c1c2)...P(cn|cn-3cn-2cn-1)

由于P(ci|ci-1)=P(ci-1ci)/P(ci-1)，P(ci-1ci)和P(ci-1)都是可以统计的量，其中，由于全集确定，P(ci)我们可以用统计次数代替，那么我们可以得到一个句子在二元模型下的概率。三元四元模型以此类推。

我们需要对语料库中所有长度为{1, 2, 3, 4}的term进行统计，作为实验的计数。

由于实际情况下，很多时候无法组出出现过的二元、三元、四元组，例如切换句子，或是验证不存在的词语。这种情况下，需要对公式进行修正。

f1(ci)=P(ci)

f2(ci)=P(ci|ci-1)

f3(ci)=P(ci|ci-1ci-2)

f4(ci)=P(ci|ci-2ci-1ci)

以四元组为例

P(ci) = a1f1+a2f2+a3f3+a4f4

这样可以避免掉不存在元组导致的0概率问题

其中a1,a2,a3,a4为参数

在以词为基准的二元模型中，我们添加了词作为语料。我们统计现有语料库的词语作为二元语料库，然后添加进一组新的参数

w2(ci)=P(ci|ci-1) // word based

P(ci) = a1f1+a2f2+b2w2

这样即可实现基于词的二元模型，三元模型类似。

### 四、实现过程

语料库的统计是直接统计

针对语料库先按非汉字字符进行切分，然后将长度为1~4的term进行个数统计，以json格式存储。

其中数据大小：一元组88KB、二元组45MB、三元组524MB、四元组1.69GB

基于词的语料库，使用jieba对句子做切分，切分后对词做二元term统计

其中针对字的语料库预处理在clean_char_data.py，针对词的语料库预处理在clean_word_data.py

推理实现基于动态规划进行

dp[i][term] = max{dp[i-1][term[:-1]] * getP(term[:-1] + ch)}

其中term长度为我们的元数，getP为上述的概率计算。

优化技巧包括，使用记忆化搜索代替全集枚举，降低搜索空间。使用滚动数组，降低存储空间，去除0概率状态，提高搜索效率。

其中基于字符的实现为char_1.py、char_2.py、char_3.py、char_4.py

基于词的实现为word_2.py

程序清单如下

```
# dataset
data/
# text data
-2016-02.txt
-2016-04.txt
-2016-05.txt
-2016-06.txt
-2016-07.txt
-2016-08.txt
-2016-09.txt
-2016-10.txt
-2016-11.txt
# pinyin to char
char.txt
# clean data
clean_char_data.py
clean_word_data.py
# inference
char_1.py
char_2.py
char_3.py
char_4.py
word_2.py
```

其中语料库被整理到data下，预处理需要使用jieba库

```shell
python3 clean_char_data.py
python3 clean_word_data.py
```

结果推理需要输入文件input.txt, 结果输出在屏幕
```shell
python3 char_1.py input.txt
python3 char_2.py input.txt
python3 char_3.py input.txt
python3 char_4.py input.txt
python3 word_2.py input.txt
```

### 五、结果分析

默认参数选择

我们针对不同等级的模型，我们递减系数

一元模型中a1=1

二元模型中a1=0.1 a2=0.9

三元模型中a1=0.01 a2=0.09 a3=0.9

四元模型中a1=0.001 a2=0.009 a3=0.09 a4=0.9

默认参数下，结果如下

一元字模型:

情化大学机算机系
我上学区了
进天会家比交万
两会在被经着开

二元字模型:

情化大学计算记系
我上学区了
今天会家比较万
两会在北京着开

三元字模型:

清华大学计算机系
我上学区了
今天会家比较万
两会在北京着开

四元字模型：

清华大学计算机系
我上学区了
今天会家比较万
两会在北京召开

我们可以看到，元数提高对精度提升还是有比较明显的提升的。例如"清华大学计算机系"，在四元中能够很好地捕捉到四字词语"清华大学"与"计算机系"。

在调整参数的时候，我们能够发现，差距调整越小，越接近上一轮的结果。而差距调整大会引入一些较为奇怪的term。现在的选择是一个比较好的选择。

而在基于词的模型中，参数选择就显得更加重要

例如在选取a1 a2 b2的时候，选取参数对结果的影响如下：

0.05 0.2 0.75:

情化大学计算机系
我上学区了
今天会家比交万
两会在北京着开


0.05 0.05 0.9:

情化大学计算机系
我上学区了
进天会家比交万
两会在北京着开

0.01 0.09 0.9

清华大学计算机系
我上学区了
今天会家比较完
两会在北京着开

0.001 0.009 0.99

清华大学计算机系
我上学区了
今天回家比较完
两会在北京朝开

分析原因，主要因为，针对同一个term，二元字模型的统计量要比词中的二元组大一个数量级左右，因此，需要手动给b2调整更大的参数。但是b2提高会导致词语中间的断点处被其他词语的term替代，导致错误。例如最后一句中的京朝。

而整体上清华大学计算机系被完整的以词语识别出来，这是基于字的二元模型做不到的了。

扩展输入集中，口语，诗句较多，需要更大的语料库支持，目前结果并不好，结果被放置在char1.res char2.res char3.res char4.res word2.res中。

### 六、总结

本文实现了基于字与词的拼音输入法，得到了比较直观的实验结果。

此实验中待改进的部分：

1. 语料库的选择，现在使用默认提供的新闻语料库，针对性不足，例如测试数据集中的口语化短语，就很难给出一个较好的结果，因为语料库中根本不存在。

2. 实现过程较为原始，使用python，难以进行多线程优化，面临搜索速度慢的问题，以及python本身带来的性能问题。如果有充足时间可以使用C/C++重构，这样能得到比较好的结果。

算法本身的理解：

1. 基于字的二元三元四元算法，从结论上区别并没有想象的那么大，我认为主要还是由于语料库的问题。

2. 基于词的效果明显比基于字的要好一些, 但在词语之间链接处仍有不适合的情况。
