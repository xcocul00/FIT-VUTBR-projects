package ija.ija2015.homework1.board;

public class Disk {
	
	private int farba=0;
	
	public Disk(boolean isWhite){
		if(!isWhite)
			this.farba=0;
		else
			this.farba=1;
	}
	
	public boolean equals(java.lang.Object obj){
		if(obj instanceof Disk)
		{
			Disk tmp = (Disk)obj;
			if(tmp.farba==farba)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	
	public void turn(){
		if(this.farba==0)
			this.farba=1;
		else
			this.farba=0;
	}
	
	public boolean isWhite(){
		if(this.farba==1)
			return true;
		else
			return false;
	}
	
	public int hashCode(){
		return this.farba;
	}
	
	
}
