#distance vector algo for 4 nodes
'''
Start with distance vectors as dicts
'''
import numpy as np
from copy import deepcopy

a = [['a',0] ,['b',3], ['c', np.inf], ['d', 6]]
b = [['a',3], ['b', 0], ['c', 2],['d',5]]
c = [['a', np.inf], ['b', 2],['c',0],['d',4]]
d = [['a',6],['b',5],['c',4],['d',0]]

router_list = [a,b,c,d]

a = np.array([0,3,np.inf, 6])
b = np.array([3,0,2,5])
c = np.array([np.inf, 2,0,4])
d = np.array([6,5,4,0])


new_router_list = deepcopy(router_list)

def dist_vector(router_list, new_router_list):
    need_updates = True
    count = 0
    changes = 0
    while need_updates:
        check_list = deepcopy(new_router_list)
        # num routers is also len list
        for i in range(len(router_list)):
            curr_router = new_router_list[i]
            ref_router = router_list[i]
            # find the comparison router
            for j in range(len(router_list[0])):
                #skip comparing to itself
                if j == i:
                    continue
                #if reachable
                if ref_router[j][1] != np.inf:
                    # get comparison router
                    comp_router = new_router_list[j]
                    # iterate each link except its own 
                    for k in range(len(comp_router)):
                        # both router vectors skip themselves as they are reciprocates
                        if k == i or k==j:
                            continue
                        new_dist = comp_router[k][1] + curr_router[j][1]
                        if new_dist < curr_router[k][1]:
                            curr_router[k][1] = new_dist
                            changes += 1
        
        #print("changes: {}\nrouters: {}".format(changes,new_router_list))
        if changes == 0:
            need_updates = False
            break
        # checklist
        if check_list == new_router_list:
            need_updates = False
            break


        
    
    return new_router_list

if __name__=='__main__':
    
    converged_list = dist_vector(router_list, new_router_list)

    for i in range(len(converged_list)):
        print("{}\n".format(converged_list[i]))