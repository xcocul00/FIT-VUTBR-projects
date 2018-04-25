package ija.ija2015.homework2.board;

public class Disk {
	
	private boolean farba;
	
	public Disk(boolean isWhite){
		this.farba=isWhite;
	}
	
	public boolean equals(java.lang.Object obj){
		if(obj instanceof Disk)
		{
			Disk tmp = (Disk)obj;
			if(this.farba==tmp.farba)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	
	public void turn(){
		this.farba = !this.farba;
	}
	
	public boolean isWhite(){
		if(this.farba==true)
			return true;
		else
			return false;
	}
	
	public int hashCode(){
		// TODO Auto-generated method stub
		return Disk.class.hashCode();
	}
	
	
}
