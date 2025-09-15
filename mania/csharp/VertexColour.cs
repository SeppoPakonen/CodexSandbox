using UnityEngine;

public class VertexColour
{
	public Color top_left = Color.white;

	public Color top_right = Color.white;

	public Color bottom_right = Color.white;

	public Color bottom_left = Color.white;

	public VertexColour()
	{
	}

	public VertexColour(Color init_color)
	{
		top_left = init_color;
		top_right = init_color;
		bottom_right = init_color;
		bottom_left = init_color;
	}

	public VertexColour(Color tl_colour, Color tr_colour, Color br_colour, Color bl_colour)
	{
		top_left = tl_colour;
		top_right = tr_colour;
		bottom_right = br_colour;
		bottom_left = bl_colour;
	}

	public VertexColour(VertexColour vert_col)
	{
		top_left = vert_col.top_left;
		top_right = vert_col.top_right;
		bottom_right = vert_col.bottom_right;
		bottom_left = vert_col.bottom_left;
	}

	public VertexColour Clone()
	{
		VertexColour vertexColour = new VertexColour();
		vertexColour.top_left = top_left;
		vertexColour.top_right = top_right;
		vertexColour.bottom_right = bottom_right;
		vertexColour.bottom_left = bottom_left;
		return vertexColour;
	}

	public VertexColour Add(VertexColour vert_col)
	{
		VertexColour vertexColour = new VertexColour();
		vertexColour.bottom_left = bottom_left + vert_col.bottom_left;
		vertexColour.bottom_right = bottom_right + vert_col.bottom_right;
		vertexColour.top_left = top_left + vert_col.top_left;
		vertexColour.top_right = top_right + vert_col.top_right;
		return vertexColour;
	}

	public VertexColour Sub(VertexColour vert_col)
	{
		VertexColour vertexColour = new VertexColour();
		vertexColour.bottom_left = bottom_left - vert_col.bottom_left;
		vertexColour.bottom_right = bottom_right - vert_col.bottom_right;
		vertexColour.top_left = top_left - vert_col.top_left;
		vertexColour.top_right = top_right - vert_col.top_right;
		return vertexColour;
	}

	public VertexColour Multiply(float factor)
	{
		VertexColour vertexColour = new VertexColour();
		vertexColour.bottom_left = bottom_left * factor;
		vertexColour.bottom_right = bottom_right * factor;
		vertexColour.top_left = top_left * factor;
		vertexColour.top_right = top_right * factor;
		return vertexColour;
	}
}
