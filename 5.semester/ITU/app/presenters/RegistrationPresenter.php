<?php 

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model\UserManager;

class RegistrationPresenter extends Nette\Application\UI\Presenter{

	private $userManager;

    public function __construct( UserManager $userManager)
    {
        $this->userManager = $userManager;
    }

    protected function createComponentRegisterForm()
    {
        $form = new Form;

        $form->addText('username', 'Uzivatelske meno:')
            ->addRule(Form::FILLED, 'Zadajte uzivatelske meno')
            ->addCondition(Form::FILLED);

        $form->addText('name', 'Meno:')
            ->addRule(Form::FILLED, 'Zadajte vase meno')
            ->addCondition(Form::FILLED);

        $form->addText('surname', 'Priezvisko:')
            ->addRule(Form::FILLED, 'Zadajte vase priezvisko')
            ->addCondition(Form::FILLED);

        $form->addPassword('password', 'Heslo:', 20)
            ->addRule(Form::FILLED, 'Prosim vyplnte svoje heslo')
            ->setOption('description', 'Heslo musi mat minimalne 6 znakov')
            ->addRule(Form::MIN_LENGTH, 'Heslo musi mat aspon %d znakov.', 6);

        $form->addPassword('password2','Heslo znovu:' ,20)
        	->addConditionOn($form['password'], Form::VALID)
        	->addRule(Form::EQUAL, 'Hesla sa nezhoduju.', $form['password'])
            ->addRule(Form::FILLED, 'Zopakujte heslo');


        $form->addSubmit('send', 'Register')
            ->setAttribute('class', 'register_button');

        $form->onSuccess[] = [$this, 'RegisterFormSucceeded'];

        return $form;
    }
    
    public function RegisterFormSucceeded($form, $values)
	{
	    try {
	    	$this->userManager->register($values);
			$this->redirect('Registration:complete');

	    } catch (Nette\Security\AuthenticationException $e) {
	        $form->addError($e->getMessage());
	    }
	}
    
}


