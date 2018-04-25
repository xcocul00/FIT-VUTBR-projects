package ija.ija2015.homework2.game;

import ija.ija2015.homework2.board.Board;
import ija.ija2015.homework2.board.Disk;
import ija.ija2015.homework2.board.Field;

public class Player {
	private boolean isWhite;
	private int disk=0;
	
	public Player(boolean isWhite){
		this.isWhite = isWhite;
	}
	public boolean isWhite(){
		if(this.isWhite==true)
		{
			return true;
		}
		else
			return false;
	}
	
	public boolean canPutDisk(Field field){
		return field.canPutDisk(new Disk(this.isWhite()));
	}
	
	public boolean emptyPool(){
		if(this.disk==0)
		{
			return true;
		}
		else
			return false;
	}
	
	public void init(Board board){
		this.disk=board.numberDisks();
		if(this.isWhite())
		{
			board.getField((board.getSize() /2), (board.getSize()/2)).putDisk(new Disk(true));
			board.getField((board.getSize() /2)+1, (board.getSize()/2)+1).putDisk(new Disk(true));
		}
		else
		{
			board.getField( (board.getSize() /2 ) ,(board.getSize() /2 )  + 1 ).putDisk(new Disk(false));
			board.getField( (board.getSize() /2 ) + 1,(board.getSize() /2 ) ).putDisk(new Disk(false));
		}
	}
	public boolean putDisk(Field field){
		this.disk --;
		return field.putDisk(new Disk(this.isWhite()));
	}
	
	@Override
	public String toString(){
		if(this.isWhite())
			return "white";
		else
			return "black";
	}
}