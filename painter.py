import matplotlib.pyplot as mpl

n = int(input())
graph = [list(map(int, input().split())) for _ in range(n)]
*perm, = map(int, input().split())
cost = float(input())
mpl.plot(
    [graph[x][0] for x in perm] +
    [graph[perm[0]][0]],
    [graph[y][1] for y in perm] +
    [graph[perm[0]][1]],
    color='orange',
    linewidth=3,
    marker='o',
    markerfacecolor='red',
    markersize=12
)
for x in perm:
	mpl.text(graph[x][0], graph[x][1], str(x), color="black", fontsize=10, )
mpl.xlim(min([x[0] for x in graph]) - 1, max([x[0] for x in graph]) + 1)
mpl.ylim(min([x[1] for x in graph]) - 1, max([x[1] for x in graph]) + 1)
mpl.title(f'cost is {cost}')
mpl.show()
