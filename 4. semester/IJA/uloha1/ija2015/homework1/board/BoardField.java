package ija.ija2015.homework1.board;

public class BoardField implements Field{
	private int row=0;
	private int col=0;
	private Field top;
	private Field down;
	private Field left;
	private Field right;
	private Field topleft;
	private Field topright;
	private Field downleft;
	private Field downright;
	private Disk disk;
	
	public BoardField(int row, int col){
		this.row=row;
		this.col=col;
	}
	public void addNextField(Direction dirs, Field field){
		switch (dirs)
		{
		case D : this.down = field;	
				 break;
		case U : this.top = field;
				 break;
		case L : this.left = field;
		 		 break;
		case R : this.right = field;
		 		 break;
		case LD : this.downleft = field;
				  break;
		case LU : this.topleft = field;
				  break;
		case RD : this.downright = field;
				  break;
		case RU : this.topright = field;
				  break;
		default : break;
		}
	}
	public boolean equals(java.lang.Object obj){
		if(obj instanceof BoardField==true)
		{
			BoardField field=(BoardField) obj;
			if(this.row == field.row && this.col==field.col)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	

	public Disk getDisk(){
		return this.disk;
	}
	
	public int hashCode(){
		return this.col+this.row;
	}

	
	public Field nextField(Direction dirs){

		switch (dirs)
		{
		case D : return this.down;					 
		case U : return this.top;
		case L : return this.left;
		case R : return this.right;
		case LD : return this.downleft;
		case LU : return this.topleft;
		case RD : return this.downright;
		case RU : return this.topright;
		default : return null;
		}
	}
	
	public boolean putDisk(Disk disk){
		if(this.disk==null)
		{
			this.disk=disk;
			return true;
		}
		else
			return false;
	}
}
