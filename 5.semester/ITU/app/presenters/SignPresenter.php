<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;

class SignPresenter extends Nette\Application\UI\Presenter{

	private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    protected function createComponentSignInForm()
    {
        $form = new Form;

        $form->addText('username')
            ->setRequired('Prosím vyplňte své uživatelské jméno.')
            ->setDefaultValue('Username')
            ->setAttribute('onfocus','this.value==this.defaultValue?this.value="":null');

        $form->addPassword('password')
            ->setRequired('Prosím vyplňte své heslo.')
            ->setDefaultValue('Password')
            ->setAttribute('onfocus','this.value==this.defaultValue?this.value="":null');

        $form->addSubmit('send', 'Log in');

        $form->onSuccess[] = [$this, 'signInFormSucceeded'];

        return $form;
    }


    public function signInFormSucceeded($form, $values)
	{
	    try {
	        $this->getUser()->login($values->username, $values->password);
	        $this->redirect('Homepage:');

	    } catch (Nette\Security\AuthenticationException $e) {
	        $form->addError('Nesprávné prihlasovacie meno alebo heslo.');
	    }
	}

	public function actionIn(){
		if ($this->getUser()->isLoggedIn()) {
        	$this->redirect('Homepage:');
    	}
	}

	public function actionOut()
	{	
	    $this->getUser()->logout();
	    $this->flashMessage('Odhlášení bylo úspěšné.');
	    $this->redirect('Sign:in');
	}

}