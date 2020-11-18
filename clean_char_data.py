import json
import jieba

if __name__ == "__main__":
    with open("data/char.txt", "r", encoding="gbk") as f:
        lines = f.readlines()
    charDict = {}
    termDict = {}
    cnt = 0
    for line in lines:
        tmp = line.strip().split()
        termDict[tmp[0]] = tmp[1:]
        for char in tmp[1:]:
            if char not in charDict:
                charDict[char] = cnt
                cnt += 1
    
    with open("term_dict.json", "w") as f:
        json.dump(termDict, f)
    with open("char_dict.json", "w") as f:
        json.dump(charDict, f)
    n = len(charDict.items())
    n += 1
    term1Cnt = {}
    term2Cnt = {}
    term3Cnt = {}
    term4Cnt = {}

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

    m = len(lines)
    k = 0
    for line in lines:
        if k % 100 == 0:
            print("%d/%d"%(k, m))
        k += 1
        text = json.loads(line)["html"]
    
        term = [n-1, n-1, n-1, n-1]
        for char in text:
            if char not in charDict:
                term = [n-1, n-1, n-1, n-1]
                continue
            v = charDict[char]
            term = term[1:]
            term.append(v)

            h = term[3]
            if h not in term1Cnt:
                term1Cnt[h] = 0
            term1Cnt[h] += 1
            h = term[2] * n + term[3]
            if h not in term2Cnt:
                term2Cnt[h] = 0
            term2Cnt[h] += 1
            h = term[1] * n * n + term[2] * n + term[3]
            if h not in term3Cnt:
                term3Cnt[h] = 0
            term3Cnt[h] += 1
            h = term[0] * n * n * n + term[1] * n * n + term[2] * n + term[3]
            if h not in term4Cnt:
                term4Cnt[h] = 0
            term4Cnt[h] += 1
    
    with open("term1_cnt.json", "w") as f:
        json.dump(term1Cnt, f)
    with open("term2_cnt.json", "w") as f:
        json.dump(term2Cnt, f)
    with open("term3_cnt.json", "w") as f:
        json.dump(term3Cnt, f)
    with open("term4_cnt.json", "w") as f:
        json.dump(term4Cnt, f)