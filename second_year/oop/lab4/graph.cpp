#include "main.h"

#include "graph.h"


// ---------------- Adjacency Matrix ----------------

AdjacencyMatrix::AdjacencyMatrix(const int width)
	: _matrix(width, std::vector<int>(width))
{
	std::cout << "AdjacencyMatrix created." << std::endl;
}


AdjacencyMatrix::~AdjacencyMatrix()
{
	std::cout << "AdjacencyMatrix destroied." << std::endl;
}


AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& m)
	: _matrix(m._matrix)
{
	std::cout << "AdjacencyMatrix copied." << std::endl;
}


const std::vector<int>& AdjacencyMatrix::operator[](const int v_index) const
{
	return get_arounding_vertices(v_index);
}


bool AdjacencyMatrix::is_linked(const int v1, const int v2) const
{
	return _matrix[v1][v2];
}


int AdjacencyMatrix::count_edges(const int v_indx) const
{
	int total = 0;

	for (auto x : _matrix)
		for (auto y : x)
			if (y == 1)
				total++;

	return total;
}


const std::vector<int>& AdjacencyMatrix::get_arounding_vertices(const int v_indx) const
{
	return _matrix[v_indx];
}


void AdjacencyMatrix::link(const int v1, const int v2)
{
	_matrix[v1][v2] = 1;
	_matrix[v2][v1] = 1;
}


void AdjacencyMatrix::link(const int v_indx, const std::vector<int>& vertices)
{
	for (auto it : vertices)
	{
		_matrix[v_indx][it] = 1;
		_matrix[it][v_indx] = 1;
	}
}


void AdjacencyMatrix::unlink(const int v1, const int v2)
{
	_matrix[v1][v2] = 0;
}


void AdjacencyMatrix::unlink(const int v_indx, const std::vector<int>& vertices)
{
	for (auto it : vertices)
	{
		_matrix[v_indx][it] = 0;
		_matrix[it][v_indx] = 0;
	}
}


void AdjacencyMatrix::show() const
{
	std::cout << "Matrix:" << std::endl;
	for (auto x : _matrix)
	{
		for (auto y : x)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


// ---------------- Incidence Matrix ----------------

IncidenceMatrix::IncidenceMatrix(const int v_num, const int e_num)
	: _matrix(v_num, std::vector<int>(e_num))
{
	std::cout << "IncidenceMatrix created." << std::endl;
}

IncidenceMatrix::~IncidenceMatrix()
{
	std::cout << "AdjacencyMatrix destroied." << std::endl;
}

IncidenceMatrix::IncidenceMatrix(const IncidenceMatrix& m)
	: _matrix(m._matrix)
{
	std::cout << "AdjacencyMatrix copied." << std::endl;
}


const std::vector<int>& IncidenceMatrix::operator[](const int v_index) const
{
	get_arounding_vertices(v_index);
}


bool IncidenceMatrix::is_linked(const int v1, const int v2) const
{
	std::cout << v1 << " : " << _matrix[v1];
	std::cout << v2 << " : " << _matrix[v2];

	for (int i = 0; i < _matrix[v1].size(); i++)
		if ((_matrix[v1][i] == 1)
			&& (_matrix[v2][i] == 1))
			return true;

	return false;
}


int IncidenceMatrix::count_edges(const int v_indx) const
{
	int total = 0;

	for (auto it : _matrix[v_indx])
		if (it == 1)
			total++;

	return total++;
}


const std::vector<int>& IncidenceMatrix::get_arounding_vertices(const int v_indx) const
{
	return _matrix[v_indx];
}

void IncidenceMatrix::link(const int v1, const int v2)
{
	if (is_linked(v1, v2))
		return;

	std::vector<int> edge(_matrix.size());

	edge[v1] = 1;
	edge[v2] = 1;

	_matrix.push_back(edge);
}

void IncidenceMatrix::link(const int v_indx, const std::vector<int>& vertices)
{
	for (auto it : vertices)
		link(v_indx, it);
}


void IncidenceMatrix::unlink(const int v1, const int v2)
{
	if (!is_linked(v1, v2))
		return;

	std::vector<int> edge(_matrix.size());

	edge[v1] = 1;
	edge[v2] = 1;

	_matrix.push_back(edge);
}


void IncidenceMatrix::unlink(const int v_indx, const std::vector<int>& vertices)
{
	for (auto it :vertices)
	{
		_matrix[v_indx][it] = 0;
		_matrix[it][v_indx] = 0;
	}
}


void IncidenceMatrix::show() const
{
	for (auto x : _matrix)
	{
		for (auto y : x)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
}
