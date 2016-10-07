Google Code Jam problem "Hexagon Game"
Problem link:https://code.google.com/codejam/contest/32014/dashboard#s=p3 

In this project, I intend to solve a Google Code Jam problem called Hexagon Game and carry out a deep study of it. The project involves not only writing a program to pass the test cases, but also studies of related knowledge, analysis of results (e.g. time and space complexity), and so on.

Problem Analysis
1. About hexagon boards and checkers
A hexagon board of size S is overall a hexagon-like graph in which there are three longest diagonals of length S. The total number of positions in a hexagon board of size S is N = (3S2+1) / 4. The aim of this game is to get the minimum score by moving checkers to one of the diagonals, which means, for any one of the three diagonals, I can find the way of movement that generates the minimum score for it. After that, I can compare the three scores and find the minimum of them again.

2. About a weighted, balanced and complete bipartite graph
Because the number of checkers is S too and at any time, there can be more than one checker on any of the hexagons (called positions afterwards), the problem can be modeled by a weighted, balanced and complete bipartite graph G. In G, T is the set containing nodes that represent target positions (i.e., hexagons on one of the three diagonals), and B is the set containing nodes that represent beginning positions of checkers. Obviously, |T| = |B| = S. Each node in T is connected by S edges to all nodes in B, and vice versa. For any edge, assume that the two nodes at its ends are t and b (belonging to T and B respectively), then the weight of the edge wbt = lbt * vbt, where lbt is the number of steps needed for the checker to start on b and end on t, and vbt is the value of that checker.

3. About minimum weighted perfect matching on the bipartite graph
With the bipartite graph abstracted from the original problem, the central work to get the solution is to carry out the minimum weighted perfect matching. It finds the perfect match of the bipartite graph where weights of all edges add up to a minimum possible value among all perfect matches.

The original problem has been analyzed and the following sub-problems are abstracted from it. Therefore, some connective tasks that are required in the original problem are missing in this re-description. The whole problem can be seen as a combination of the sub-problems and the connective tasks.
Hexagonal board: arrange hexagons to form a bigger hexagon-like graph.
Sub-problem 1: in a hexagonal board, for one step, it is allowed to move from a hexagon to one of its neighbors. Find a way to compute how many steps are needed in the movement that starts and ends at any two hexagons.
Sub-problem 2: in a weighted, balanced and complete bipartite graph, find the minimum weighted perfect match.

Sub-problem 1 can be solved using Breadth-first search (no need to employ the Dijkstra's Algorithm or the Floyd-Warshall Algorithm), and sub-problem 2 - the Kuhn-Munkres Algorithm.

Overall time complexity: O(S^3).

Reference list:
[1] "Problem D. Hexagon Game." Code Jam Beta 2008 - Google Code Jam. Google. <https://code.google.com/codejam/contest/32014/dashboard#s=p3>.
[2] "Bipartite Graph." Wikipedia. Wikimedia Foundation. <https://en.wikipedia.org/wiki/Bipartite_graph>.
[3] Fengyang0317. "Code Jam Beta 2008." CSDN Blog. <http://blog.csdn.net/fengyang0317/article/details/24106087>.
[4] "Computing Distances on a Hexagon Network."  <http://52xenos.blogspot.ca/2013/07/hexagon-network.html>.
[5] Song, Renfei. "Bipartite Matching." <https://www.renfei.org/blog/bipartite-matching.html>.
[6] “A Summary of bipartite Matching Algorithms.” <http://www.360doc.com/content/11/0718/14/3701281_134273282.shtml>
