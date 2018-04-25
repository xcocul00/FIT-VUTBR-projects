package ija.ija2015.homework2.board;

public class BoardField implements Field{
	private int row;
	private int col;
	private Disk disk = null;
	private Field field []= new Field[8];
	private boolean occupy = false;
	
	public BoardField(int row, int col){
		this.row=row;
		this.col=col;
	}
	public void addNextField(Direction dirs, Field field){
		switch (dirs)
		{
		case D : this.field[0] = field;	
				 break;
		case L : this.field[1] = field;
				 break;
		case R : this.field[2] = field;
		 		 break;
		case U : this.field[3] = field;
		 		 break;
		case LD : this.field[4] = field;
				  break;
		case LU : this.field[5] = field;
				  break;
		case RD : this.field[6] = field;
				  break;
		case RU : this.field[7] = field;
				  break;
		default : break;
		}
	}
	public boolean equals(java.lang.Object obj){
		if(obj instanceof BoardField==true)
		{
			BoardField tmpField=(BoardField) obj;
			if(this.row == tmpField.row && this.col==tmpField.col)
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
		case D : return this.field[0];					 
		case L : return this.field[1];
		case R : return this.field[2];
		case U : return this.field[3];
		case LD : return this.field[4];
		case LU : return this.field[5];
		case RD : return this.field[6];
		case RU : return this.field[7];
		default : return null;
		}
	}
	
	public boolean putDisk(Disk disk){
		if(this.occupy==false)
		{
			this.disk=disk;
			this.occupy=true;
			return true;
		}
		else
			return false;
	}
	
	public boolean isEmpty() {
		if(this.occupy==false)
		{
			return true;
		}
		else
			return false;
	}
	
	public boolean canPutDisk(Disk d){
		int i;
		for(i=0;i<4;i++)
		{
			if(this.field[i].isEmpty()==false)
			{
				if(this.field[i].getDisk().isWhite() != d.isWhite())
				{
					this.field[i].getDisk().turn();
					return true;
				}
			}
		}
		return false;
	}
}
