import sys


def bisect_search(col, target, start=None, trans=lambda x: x):
    debug = 0
    iters = 0

    n = len(col)
    if target < trans(col[0]):
        return 0
    else:
        idx = start if start is not None else int(n / 2)
        bounce = max(int(idx / 2), 1)
        if debug > 10:
            print(f"bisect_search, target: {target}, idx: {idx}, n: {n}, bounce: {bounce}, raw col: {col}")
        # bounce over
        while trans(col[idx]) < target:
            iters += 1
            if debug > 10:
                print(f"idx: {idx}, raw col: {col}, while {trans(col[idx])} < {target}")
            if idx + bounce < n and trans(col[idx + bounce]) <= target:
                idx += bounce
            elif bounce > 1:
                bounce = int(bounce / 2)
            elif idx + 1 == n or trans(col[idx + 1]) > target:
                if idx + 1 < n:
                    idx += 1
                break
        while trans(col[idx]) > target:
            iters += 1
            if debug > 10:
                print(f"idx: {idx}, raw col: {col}, while {trans(col[idx])} > {target}")
            if idx - bounce >= 0 and trans(col[idx - bounce]) >= target:
                idx -= bounce
            elif bounce > 1:
                bounce = int(bounce / 2)
            elif idx - 1 == 0 or trans(col[idx - 1]) < target:
                break

        if debug >= 0:
            print(f"bisect_search, target: {target}, idx: {idx} = {trans(col[idx])}, surrounding: [..{str(trans(col[idx - 1])) + ', ' if idx - 1 >= 0 else ''}{trans(col[idx])}{', ' + str(trans(col[idx + 1])) if idx + 1 < n else ''}..]")

        return idx


if __name__ == "__main__":
    search = sys.argv[1]
    args = sys.argv[2:]
    print(bisect_search(args, search))
