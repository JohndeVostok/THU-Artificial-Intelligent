import json
import math
import sys

if __name__ == "__main__":
    with open("term_dict.json", "r") as f:
        termDict = json.load(f)
    with open("char_dict.json", "r") as f:
        charDict = json.load(f)
    with open("pair_cnt.json", "r") as f:
        pairCnt = json.load(f)
    with open("term1_cnt.json", "r") as f:
        term1Cnt = json.load(f)
    with open(sys.argv[1], "r") as f:
        lines = f.readlines()

    a0 = 0.5
    a1 = 0.5

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
        res = ""
        for i in range(len(terms)):
            term = terms[i]
            mx = -1e9
            ch = ""
            for char in termDict[term]:
                v = charDict[char]
                cnt = 1
                if str(v) in term1Cnt:
                    cnt += term1Cnt[str(v)]
                if cnt > mx:
                    mx = cnt
                    ch = char
            res += ch
        print(res)
