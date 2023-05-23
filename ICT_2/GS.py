from collections import deque

#Stu = [1,2,3,4,5,6]
Stu = [1,2,3,4,5,6,7,8]
Labs = [1,2,3]

#pre_stu_to_lab = [[1],[1,3],[2,1,3],[1,2,3],[3,2,1],[1,2,3]]
#pre_lab_to_stu = [[5,3,2,6,4],[2,6,3,5],[2,1,3,6,4,5]]

pre_stu_to_lab = [[1],[1,2],[2,1,3],[2,1,3],[1,3,2],[3,1,2],[1,2,3],[2,1,3]]
pre_lab_to_stu = [[1,2,4,8,3,6,7],[1,7,4,5,3,2],[2,1,8,6,4,5,3,7]]

#lab_capacity = {1:2,2:1,3:4}
lab_capacity = {1:2,2:4,3:2}
def find_lab(Stu, Labs, pre_stu_to_lab, pre_lab_to_stu):
    stu_num = len(Stu)
    lab_num = len(Labs)
    current_Stu = { i+1:None for i in range(stu_num)}
    current_Lab = { i+1:[] for i in range(lab_num)}
    new_match = {}
    
    for i in range(stu_num):
        new_match[Stu[i]] = deque(pre_stu_to_lab[i])
    
    sort_lab = {}
    for i in range(lab_num):
        sort_lab[Labs[i]] = pre_lab_to_stu[i]
    cnt = 0
    while cnt < stu_num*stu_num:
        for i in range(stu_num):
            sid = Stu[i]
            if current_Stu[sid] or len(new_match[sid])==0: continue
            else:
                select = new_match[sid][0] #string eg."L1"
                lab = sort_lab[select] # dict
                if sid in lab: # lab can accept this student
                    if len(current_Lab[select]) == 0 or lab_capacity[select]:
                        current_Stu[sid] = select
                        current_Lab[select].append(sid)
                        new_match[sid].popleft()
                        lab_capacity[select] -= 1
                    else:
                        indice = [lab.index(val) for val in current_Lab[select]]
                        substitute = max(indice)
                        if substitute < lab.index(sid): # preference is lower 
                            new_match[sid].popleft()
                        else:
                            rm = current_Lab[select].index(lab[substitute]) # find index of element we want to remove
                            current_Stu[current_Lab[select][rm]]= None # modify students
                            current_Stu[sid] = select
                            
                            current_Lab[select].remove(current_Lab[select][rm]) # modify labs
                            current_Lab[select].append(sid)
                            new_match[sid].popleft()
                else:
                    new_match[sid].popleft() # can't accept this studen
        cnt += 1
    return current_Lab

if __name__=='__main__':
    print(find_lab(Stu, Labs,pre_stu_to_lab,pre_lab_to_stu))