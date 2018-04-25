package ija.ija2015.homework1.board;

public class Board {
	Field board[][] = null;
	int size=0;
	
	public Board(int size){
		this.board=new Field[size+2][size+2];
		this.size=size;
		for(int i=0;i<size+2;i++){
			for(int j=0;j<size+2;j++){
				if(i==0 || j==0 || i==size+1 || j==size+1)
					board[i][j]=new BorderField();
				else
					board[i][j]=new BoardField(i,j);
			}}
		for(int i=1; i<=size; i++){
			for(int j=1; j<=size; j++){
					board[i][j].addNextField(Field.Direction.L, board[i][j-1]);
					board[i][j].addNextField(Field.Direction.R, board[i][j+1]);
					board[i][j].addNextField(Field.Direction.D, board[i+1][j]);
					board[i][j].addNextField(Field.Direction.U, board[i-1][j]);
					board[i][j].addNextField(Field.Direction.LD, board[i+1][j-1]);
					board[i][j].addNextField(Field.Direction.LU, board[i-1][j-1]);
					board[i][j].addNextField(Field.Direction.RD, board[i+1][j+1]);
					board[i][j].addNextField(Field.Direction.RU, board[i-1][j+1]);
			}}
	}
	public Field getField(int row, int col){
		return this.board[row][col];
	}
	public int getSize(){
		return this.size;
	}

}
