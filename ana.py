import json
import math

if __name__ == "__main__":
    with open("term_dict.json", "r") as f:
        termDict = json.load(f)
    with open("char_dict.json", "r") as f:
        charDict = json.load(f)
    with open("char_cnt.json", "r") as f:
        charCnt = json.load(f)
    with open("pair_cnt.json", "r") as f:
        pairCnt = json.load(f)

    # with open("input.txt", "r") as f:
    #     lines = f.readlines()
    with open("test_input.txt", "r") as f:
        lines = f.readlines()[::2]

    alpha = 0.8

    for line in lines:
        terms = line.strip().split()
        flag = False
        pre = []
        for term in terms:
            if term not in termDict:
                flag = True
                print("invalid term.")
                break
        for i in range(len(terms)):
            term = terms[i]
            if i == 0:
                cur = []
                for char in termDict[term]:
                    p = charCnt[charDict[char]] + 1
                    cur.append([char, char, p])
            else:
                cur = []
                for char in termDict[term]:
                    max = -1e9
                    preChar = ""
                    v = charDict[char]
                    p0 = (1 - alpha) * (charCnt[v] + 1)
                    for tmp in pre:
                        u = charDict[tmp[0]]
                        if u * 6763 + v not in pairCnt:
                            p = p0
                        else:
                            p = p0 + alpha * (pairCnt[u * 6763 + v] + 1) / (charCnt[u] + 1)
                        if tmp[2] * p > max:
                            preChar = tmp[1]
                            max = tmp[2] * p
                    preChar += char
                    cur.append([char, preChar, max])
            pre = cur
        max = -1e9
        res = ""
        for tmp in pre:
            if tmp[2] > max:
                max = tmp[2]
                res = tmp[1]
        print(res)
