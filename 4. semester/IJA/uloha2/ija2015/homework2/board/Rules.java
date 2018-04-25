package ija.ija2015.homework2.board;

public interface Rules{
	public int getSize();
	public int numberDisks();
	public Field createField(int row, int col);
}