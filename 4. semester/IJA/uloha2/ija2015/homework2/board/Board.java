package ija.ija2015.homework2.board;

public class Board {
	Field board[][] = null;
	private int size;
	private int disk=0;
	
	public Board(Rules rules){
		this.size=rules.getSize();
		this.board=new Field[this.size+2][this.size+2];
		
		for(int i=0;i<this.size+2;i++){
			for(int j=0;j<this.size+2;j++){
				if(i==0 || j==0 || i==this.size+1 || j==this.size+1)
					board[i][j]=new BorderField();
				else
					board[i][j]=rules.createField(i, j);
			}}
		for(int i=1; i<=this.size; i++){
			for(int j=1; j<=this.size; j++){
					board[i][j].addNextField(Field.Direction.L, board[i][j-1]);
					board[i][j].addNextField(Field.Direction.R, board[i][j+1]);
					board[i][j].addNextField(Field.Direction.D, board[i+1][j]);
					board[i][j].addNextField(Field.Direction.U, board[i-1][j]);
					board[i][j].addNextField(Field.Direction.LD, board[i+1][j-1]);
					board[i][j].addNextField(Field.Direction.LU, board[i-1][j-1]);
					board[i][j].addNextField(Field.Direction.RD, board[i+1][j+1]);
					board[i][j].addNextField(Field.Direction.RU, board[i-1][j+1]);
			}}

		this.disk = rules.numberDisks();
	}
	public Field getField(int row, int col){
		return this.board[row][col];
	}
	public int getSize(){
		return this.size;
	}
	public int numberDisks(){
		return this.disk;
	}

}
