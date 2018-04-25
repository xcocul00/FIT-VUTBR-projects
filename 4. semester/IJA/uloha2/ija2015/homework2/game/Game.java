package ija.ija2015.homework2.game;

import ija.ija2015.homework2.board.Board;

public class Game {
	private Board board;
	private Player white;
	private Player black;
	private Player current;
	private boolean is_white=true;
	private boolean is_black=true;
	
	public Game (Board board){
		this.board=board;
	}
	
	public boolean addPlayer(Player player){
		if(this.is_white==true)
		{
			this.is_white=false;
			this.white=player;
			this.white.init(this.board);
			this.current=this.white;
			return true;
		}
		if(this.is_black==true){
			this.is_black=false;
			this.black=player;
			this.black.init(this.board);
			return true;
		}
		return false;
	}
	public Player currentPlayer(){
		return this.current;
	}
	public Player nextPlayer(){
		if(current.equals(this.black)){
			this.current = this.white;
			return currentPlayer();
		}
		else
		{
			this.current = this.black;
			return currentPlayer();
		}
	}
	public Board getBoard(){
		return this.board;
	}
}