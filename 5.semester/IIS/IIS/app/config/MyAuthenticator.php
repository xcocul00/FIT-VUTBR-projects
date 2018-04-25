<?php

use Nette\Security as NS;

class MyAuthenticator extends Nette\Object implements NS\IAuthenticator{

	public $database;

	function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	function authenticate(array $credentials){
		
		list($username, $password) = $credentials;
		$row = $this->database->table('Host_hotela')
			->where('Login', $username)->fetch();

		if(! $row){
			$row = $this->database->table('Zamestnanec')
				->where('Login', $username)->fetch();

			if(! $row){
				throw new NS\AuthenticationException('User not found.');
			}

			$id = $row->Zamestnanec_id;

		}else{
			$id = $row->Host_id;
		}


		if(! ($password == $row->Password) ){
			throw new  NS\AuthenticationException("Invalid password");
		}

		return new NS\Identity($id, $row->Role, ['username' => $row->Login]);
	}
}
