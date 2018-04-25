package ija.ija2015.homework1.board;

public interface Field {

		void addNextField(Field.Direction dirs, Field field);
		Disk getDisk();
		Field nextField(Field.Direction dirs);
		boolean putDisk(Disk disk);
			
		enum Direction{
			D,L,R,U,LD,LU,RD,RU;
		}
}
