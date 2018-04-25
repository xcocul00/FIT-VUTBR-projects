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
            ->setRequired('Prosím zadajte svoje uživateľské meno.')
            ->setAttribute('placeholder',"Prihlasovacie meno")
            ->setAttribute('onfocus','this.value==this.defaultValue?this.value="":null');

        $form->addPassword('password')
            ->setRequired('Prosím zadajte svoje heslo.')
            ->setAttribute('placeholder',"Heslo")
            ->setAttribute('onfocus','this.value==this.defaultValue?this.value="":null');

        $form->addSubmit('send', 'Prihlásiť sa');

        $form->onSuccess[] = [$this, 'signInFormSucceeded'];

        return $form;
    }


    public function signInFormSucceeded($form, $values)
	{
	    try {
	        $this->getUser()->login($values->username, $values->password);
	        $this->redirect('Homepage:');

	    } catch (Nette\Security\AuthenticationException $e) {
	        $form->addError('Nesprávne prihlasovacie meno alebo heslo.');
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
	    $this->flashMessage('Boli ste úspešne odhlasený.');
	    $this->redirect('Sign:in');
	}

}