package ija.ija2015.homework2.game;

import ija.ija2015.homework2.board.BoardField;
import ija.ija2015.homework2.board.Field;
import ija.ija2015.homework2.board.Rules;

public class ReversiRules implements Rules{
	
	private int size;
	private int diskNumber;
	
	public ReversiRules(int size){
		this.size=size;
		this.diskNumber=(this.size * this.size)/2;
	}
	
	public Field createField(int row, int col){
		BoardField tmp = new BoardField(row,col);
		return tmp;
	}
	
	public int getSize(){
		return this.size;
	}
	public int numberDisks(){
		return this.diskNumber;
	}




}
