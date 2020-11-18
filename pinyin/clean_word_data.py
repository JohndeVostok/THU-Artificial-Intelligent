import json
import jieba

if __name__ == "__main__":
    lines = []
    with open("data/2016-02.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-04.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-05.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-06.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-07.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-08.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-09.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-10.txt", "r", encoding="gbk") as f:
        lines += f.readlines()
    with open("data/2016-11.txt", "r", encoding="gbk") as f:
        lines += f.readlines()

    wordCnt = {}
    termCnt = {}
    m = len(lines)
    k = 0
    for line in lines:
        if k % 100 == 0:
            print("%d/%d"%(k, m))
        k += 1
        text = json.loads(line)["html"]
        wordList = jieba.lcut(text)
        for word in wordList:
            if word not in wordCnt:
                wordCnt[word] = 0
            wordCnt[word] += 1
    
    for word in wordCnt:
        l = len(word)
        for i in range(l - 2):
            term = word[i:i+2]
            if term not in termCnt:
                termCnt[term] = 0
            termCnt[term] += wordCnt[word]

    with open("term2_word_cnt.json", "w") as f:
        json.dump(termCnt, f)
