#include <cstdio>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#define lson o << 1 
#define rson o << 1 | 1 
#define mid (l + r) / 2 

const std::string INPUT_FILE_NAME = "INPUT.TXT";

enum class PointType
{
	Begin,
	End,
	BeginEnd,
	IntersectionBegin,
	IntersectionEnd,
	IntersectionBeginEnd,
};

struct Point
{
	int x;
	int y;
	PointType type;

	friend std::istream& operator>>(std::istream& is, Point& point)
	{
		is >> point.x >> point.y;
		return is;
	}
};

struct Edge
{
	int left;
	int right;
	int height;
	int flag;
}e[10005];

struct Tree
{
	int sum;
	int num;
	int len;
	bool lflag;
	bool rflag;
}tree[100005];
int n, mx = -2147483647, mn = 2147483647, edgenum, ans, last;
void add_edge(int l, int r, int h, int f)
{
	e[++edgenum].left = l;
	e[edgenum].right = r;
	e[edgenum].height = h;
	e[edgenum].flag = f;
}
bool cmp(Edge a, Edge b)
{
	return a.height < b.height || a.height == b.height && a.flag > b.flag;
	
}

void pushup(int o, int l, int r)
{
	if (tree[o].sum)
	{
		tree[o].num = 1;
		tree[o].len = r - l + 1;
		tree[o].lflag = tree[o].rflag = 1;
	}
	else if (l == r)
	{
		tree[o].len = 0;
		tree[o].num = 0;
		tree[o].lflag = tree[o].rflag = 0;
	}
	else
	{
		tree[o].len = tree[lson].len + tree[rson].len;
		tree[o].num = tree[lson].num + tree[rson].num;
		if (tree[lson].rflag && tree[rson].lflag) tree[o].num--;
		tree[o].lflag = tree[lson].lflag;
		tree[o].rflag = tree[rson].rflag;
	}
}
void add(int o, int l, int r, int from, int to, int value)
{
	if (l >= from && r <= to)
	{
		tree[o].sum += value;
		pushup(o, l, r);
		return;
		
	}
	if (from <= mid) add(lson, l, mid, from, to, value);
	if (to > mid) add(rson, mid + 1, r, from, to, value);
	pushup(o, l, r);
}

struct Dimensions
{
	int width;
	int height;
};

bool ValidateFile(const std::ifstream& inputFile);
Dimensions GetDimensions(Point& A, Point& C);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	int N; //rectangles number
	inputFile >> N;

	for (int i = 0; i < N; ++i)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		Point D = { A.x, A.y + dimensions.height };

		mx = std::max(mx, std::max(D.x, B.x));
		mn = std::min(mn, std::min(D.x, B.x));
		add_edge(D.x, B.x, D.y, 1);
		add_edge(D.x, B.x, B.y, -1);
	}

	if (mn <= 0)
	{
		for (int i = 1; i <= edgenum; i++)
		{
			e[i].left += -mn + 1;
			e[i].right += -mn + 1;
			
		}
		mx -= mn;
	}
	std::sort(e + 1, e + edgenum + 1, cmp);
	for (int i = 1; i <= edgenum; i++)
	{
		add(1, 1, mx, e[i].left, e[i].right - 1, e[i].flag);
		while (e[i].height == e[i + 1].height && e[i].flag == e[i + 1].flag)
		{
			i++;
			add(1, 1, mx, e[i].left, e[i].right - 1, e[i].flag);
			
		}
		ans += abs(tree[1].len - last);
		last = tree[1].len;
		ans += tree[1].num * 2 * (e[i + 1].height - e[i].height);
		
	}
	std::cout << ans << std::endl;
	return 0;
	
}

bool ValidateFile(const std::ifstream& inputFile)
{
	if (!inputFile.is_open())
	{
		std::cout << "Failed to open input file for reading\n";

		return false;
	}

	return true;
}

Dimensions GetDimensions(Point & A, Point & C)
{
	Dimensions result;
	result.width = C.x - A.x;
	result.height = C.y - A.y;

	return result;
}
