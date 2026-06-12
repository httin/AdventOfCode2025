#!/run/current-system/sw/bin/python3

input = open("day12.txt", "r").read().split("\n\n")

presents = input[:-1]
regions = input[-1].splitlines()

size = {}
for present in presents:
    lines: list[str] = present.splitlines()
    idx = int(lines[0][:-1])
    count = sum(line.count("#") for line in lines[1:])
    size[idx] = count

ans = 0
for region in regions:
    colon = region.index(":")
    [m, n] = list(map(int, region[:colon].split("x")))
    nums = list(map(int, region[colon + 1:].split()))
    required = sum(num * size[idx] for idx, num in enumerate(nums))
    if m*n > required * 1.2:
        ans += 1

print(ans)