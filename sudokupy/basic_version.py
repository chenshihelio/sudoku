import numpy as np 

verbose = True
stepbystep = True
if not stepbystep:
    verbose = False

N = 9

statInit = 0
statFix = 1
statTry = 2
statUnknown = -1
solFail = 0
solSuccess = 1

fileName = "input.txt"

def writeArr(arr):
    # print(" -----------------------")
    # for i in range(N):
    #     for j in range(N):
    #         if j == 0:
    #             print(f"| {arr[i,j]} ", end="")
    #         elif j == 3 or j == 6:
    #             print(f"| {arr[i,j]} ", end="")
    #         elif j == 8:
    #             print(f"{arr[i,j]} |")
    #         else:
    #             print(arr[i,j], end=" ")
    #     if i == 2 or i == 5:
    #         print("|-----------------------|")
    # print(" -----------------------")

    print("    1 2 3   4 5 6   7 8 9 ")
    # print("    | | |   | | |   | | | ")
    print("   -----------------------")
    for i in range(N):
        print('{:d}-'.format(i+1),end='')
        for j in range(N):
            if j == 0:
                print(f"| {arr[i,j]} ", end="")
            elif j == 3 or j == 6:
                print(f"| {arr[i,j]} ", end="")
            elif j == 8:
                print(f"{arr[i,j]} |")
            else:
                print(arr[i,j], end=" ")
        if i == 2 or i == 5:
            print("  |-----------------------|")
    print("   -----------------------")

def readArr(arr, fileName):
    with open(fileName, 'r') as file:
        for i in range(N):
            row = list(map(int, file.readline().split()))
            for j in range(N):
                arr[i,j] = row[j]



def reset_indicator(arr):
    for i in range(len(arr)):
        arr[i] = 0

def get_all_possible_numbers(indicator):
    possible_numbers = []
    for i in range(len(indicator)):
        if indicator[i] == 0:
            possible_numbers.append(i + 1)
    return possible_numbers

def check_whether_finish(stat):
    number_unknown = 0
    for i in range(N):
        for j in range(N):
            if stat[i,j] == statUnknown:
                number_unknown = number_unknown + 1
    if number_unknown == 0:
        return True
    else:
        return False

def solve(arr, stat, countPossible):
    if stepbystep:
        print('\nCurrent Matrix = ')
        writeArr(arr)
        print('\n')
        input('Press any key to continue......')

    indicator = np.zeros(N, dtype=int)
    countAllPossibleNumbers = 0

    for i in range(N):
        for j in range(N):
            if stat[i,j] == statUnknown:
                reset_indicator(indicator)

                for i1 in range(N):
                    if stat[i1,j] != statUnknown:
                        indicator[arr[i1,j] - 1] = 1
                      
                for j1 in range(N):
                    if stat[i,j1] != statUnknown:
                        indicator[arr[i,j1] - 1] = 1
                       
                iBox = i // 3
                jBox = j // 3

                for i1 in range(3):
                    for j1 in range(3):
                        i2 = iBox * 3 + i1
                        j2 = jBox * 3 + j1

                        if stat[i2,j2] != statUnknown:
                            indicator[arr[i2,j2] - 1] = 1
                           
                totalOccupied = np.sum(indicator)
                totalUnoccupied = N - totalOccupied

                if totalUnoccupied == 0:
                    if verbose:
                        print('\nDeadend at ({:d},{:d})={:d}... \n\n'.format(i+1,j+1,arr[i,j]))
                    return solFail
                elif totalUnoccupied == 1:
                    possibleNumber = get_all_possible_numbers(indicator)
                    countAllPossibleNumbers = len(possibleNumber)
                    arr[i,j] = possibleNumber[0]
                    stat[i,j] = statTry
                    countPossible[i,j] = 0
                    if verbose:
                        print('\nFill ({:d},{:d})={:d}'.format(i+1,j+1,possibleNumber[0]))

                    whetherFinish = check_whether_finish(stat)
                    if whetherFinish == 1:
                        stat[i,j] = statFix
                        return solSuccess

                    resultSol = solve(arr, stat, countPossible)
                    if resultSol == solFail:
                        if verbose:
                            print('\nDeadend at ({:d},{:d})={:d}... \n'.format(i+1,j+1,arr[i,j]))
                        stat[i,j] = statUnknown
                        arr[i,j] = 0
                        return solFail
                    elif resultSol == solSuccess:
                        if verbose:
                            print('\nFix ({:d},{:d})={:d} \n\n'.format(i+1,j+1,arr[i,j]))
                        stat[i,j] = statFix
                        return solSuccess
                else:
                    possibleNumber = get_all_possible_numbers(indicator)
                    countAllPossibleNumbers = len(possibleNumber)
                    countPossible[i,j] = countAllPossibleNumbers


    imin = -1
    jmin = -1
    countMin = N + 1
    for i in range(N):
        for j in range(N):
            if (stat[i,j] == statUnknown) and (countPossible[i,j] < countMin):
                countMin = countPossible[i,j]
                imin = i
                jmin = j

    reset_indicator(indicator)

    for i1 in range(N):
        if stat[i1,jmin] != statUnknown:
            indicator[arr[i1,jmin] - 1] = 1

    for j1 in range(N):
        if stat[imin,j1] != statUnknown:
            indicator[arr[imin,j1] - 1] = 1

    iBox = imin // 3
    jBox = jmin // 3

    for i1 in range(3):
        for j1 in range(3):
            i2 = iBox * 3 + i1
            j2 = jBox * 3 + j1

            if stat[i2,j2] != statUnknown:
                indicator[arr[i2,j2] - 1] = 1

    possibleNumber = get_all_possible_numbers(indicator)
    countAllPossibleNumbers = len(possibleNumber)
    stat[imin,jmin] = statTry

    if verbose:
        print('\nBegin to try ({:d},{:d})..\n'.format(imin+1,jmin+1))

    for ic in range(countAllPossibleNumbers):
        arr[imin,jmin] = possibleNumber[ic]
        resultSol = solve(arr, stat, countPossible)

        if resultSol == solSuccess:
            if verbose:
                print('\nFix ({:d},{:d})={:d} \n'.format(imin+1,jmin+1,arr[imin,jmin]))
            stat[imin,jmin] = statFix
            return solSuccess
        if resultSol == solFail:
            if verbose:
                print('\nFail at ({:d},{:d})={:d}, try next number \n'.format(imin+1,jmin+1,arr[imin,jmin]))
            continue

    if verbose:
        print('\nDeadend at ({:d},{:d})... \n'.format(imin+1,jmin+1))
    stat[imin,jmin] = statUnknown
    return solFail

def main():
    arr = np.zeros([N,N],dtype=int)
    stat =  np.zeros([N,N],dtype=int)
    countPossible =  np.zeros([N,N],dtype=int)

    readArr(arr, fileName)
    print("Input matrix is : ")
    writeArr(arr)

    

    for i in range(N):
        for j in range(N):
            if arr[i,j] == 0:
                stat[i,j] = statUnknown
            else:
                stat[i,j] = statInit

    if verbose:
        print('Begin to solve ......')

    resultSol = solve(arr, stat, countPossible)

    if resultSol == solFail:
        print("Cannot find a solution!!!")
        return 1
    elif resultSol == solSuccess:
        print("\n\nThe solution to this sudoku is:\n")
        writeArr(arr)

if __name__ == "__main__":
    main()