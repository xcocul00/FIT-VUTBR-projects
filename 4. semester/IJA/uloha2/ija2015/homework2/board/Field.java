package ija.ija2015.homework2.board;

public interface Field {

		void addNextField(Field.Direction dirs, Field field);
		Disk getDisk();
		Field nextField(Field.Direction dirs);
		boolean putDisk(Disk disk);
		boolean isEmpty();
		boolean canPutDisk(Disk d);
			
		enum Direction{
			D,L,R,U,LD,LU,RD,RU;
		}
}
