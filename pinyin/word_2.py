import json
import math
import sys

a1 = 0.001
a2 = 0.009
b2 = 0.99

def getP(term):
    v0 = charDict[term[0]]
    v1 = charDict[term[1]]
    p1 = 1
    h = str(v1)
    if h in term1Cnt:
        p1 += term1Cnt[h]
    p2 = 1
    h = str(v0 * n + v1)
    if h in term2Cnt:
        p2 += term2Cnt[h]
    p3 = 1
    if term in wordCnt:
        p3 += wordCnt[term]
    return a1 * p1 + a2 * p2 + b2 * p3
    


if __name__ == "__main__":
    with open("term_dict.json", "r") as f:
        termDict = json.load(f)
    with open("char_dict.json", "r") as f:
        charDict = json.load(f)
    with open("pair_cnt.json", "r") as f:
        pairCnt = json.load(f)
    with open("term1_cnt.json", "r") as f:
        term1Cnt = json.load(f)
    with open("term2_cnt.json", "r") as f:
        term2Cnt = json.load(f)
    with open("term2_word_cnt.json", "r") as f:
        wordCnt = json.load(f)
    # with open(sys.argv[1], "r") as f:
    #     lines = f.readlines()
    with open("test_input.txt", "r") as f:
        lines = f.readlines()[::2]


    n = len(charDict)
    charDict["*"] = n
    n += 1

    for line in lines:
        terms = line.strip().split()
        flag = False
        for term in terms:
            if term not in termDict:
                flag = True
                print("invalid term.")
                break
        if flag:
            continue
        dp0 = {"**": ["", 1]}
        for i in range(len(terms)):
            pinyin = terms[i]
            dp1 = {}
            mx = -1e9
            for term in dp0:
                tmp = dp0[term]
                for char in termDict[pinyin]:
                    newTerm = term[1:] + char
                    if newTerm not in dp1:
                        dp1[newTerm] = ["", -1e9]
                    p = getP(newTerm)
                    if tmp[1] * p > dp1[newTerm][1]:
                        dp1[newTerm][0] = tmp[0] + char
                        dp1[newTerm][1] = tmp[1] * p
            dp0 = dp1
        mx = -1e9
        res = ""
        for term in dp1:
            if dp1[term][1] > mx:
                mx = dp1[term][1]
                res = dp1[term][0]
        print(res)
        