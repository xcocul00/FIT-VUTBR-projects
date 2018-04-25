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

        $form->addText('username', 'Uživateľské meno:', 10)
            ->addRule(Form::FILLED, 'Zadajte uživateľské meno')
            ->addRule(Form::MAX_LENGTH, 'Uživateľské meno môže mať maximálne %d znakov.', 10)
            ->setOption('description', 'Meno pod ktorým sa budete prihlasovať do systému')
            ->addCondition(Form::FILLED);

        $form->addPassword('password', 'Heslo:', 10)
            ->addRule(Form::FILLED, 'Prosím vyplnte svoje heslo')
            ->setOption('description', 'Heslo musí mať minimálne 6 znakov')
            ->addRule(Form::MIN_LENGTH, 'Heslo musi mať aspoň %d znakov.', 6)
            ->addRule(Form::MAX_LENGTH, 'Heslo môže mať maximálne %d znakov.', 10);

        $form->addPassword('password2','Heslo znovu:', 10)
            ->addRule(Form::EQUAL, 'Hesla sa nezhodujú.', $form['password'])
            ->addRule(Form::FILLED, 'Zopakujte heslo')
            ->addRule(Form::MAX_LENGTH, 'Heslo môže mať maximálne %d znakov.', 10)
            ->addConditionOn($form['password'], Form::VALID);

        $form->addText('name', 'Meno:', 20)
            ->addRule(Form::FILLED, 'Zadajte vaše meno')
            ->addRule(Form::MAX_LENGTH, 'Meno môže mať maximálne %d znakov.', 20)
            ->addCondition(Form::FILLED);

        $form->addText('surname', 'Priezvisko:', 20)
            ->addRule(Form::FILLED, 'Zadajte vaše priezvisko')
            ->addRule(Form::MAX_LENGTH, 'Meno môže mať maximálne %d znakov.', 20)
            ->addCondition(Form::FILLED);


        $form->addText('email', 'Email:',50)
            ->addRule(Form::FILLED, 'Zadajte email')
            ->addRule(Form::EMAIL, 'Email nemá správny formát')
            ->addRule(Form::MAX_LENGTH, 'E-mail môže mať maximálne %d znakov.', 50);

        $form->addText('cislo_op', 'Číslo dokladu:',20)
            ->addRule(Form::FILLED, 'Zadajte číslo dokladu totožnosti')
            ->addRule(Form::MAX_LENGTH, 'Číslo dokladu môže mať maximálne %d znakov.', 20);

        $form->addSubmit('send', 'Vytvoriť účet')
            ->setAttribute('class', 'register_button');
            
        $form->onSuccess[] = [$this, 'RegisterFormSucceeded'];

        return $form;
    }
    
    public function actionDefault(){
        if ($this->getUser()->isLoggedIn()) {
            $this->redirect('Homepage:');
        }
    }

    public function RegisterFormSucceeded($form, $values)
	{
	    try {
	    	$this->userManager->register_guest($values);
			$this->redirect('Registration:complete');

	    } catch (Nette\Security\AuthenticationException $e) {
	        $form->addError($e->getMessage());
	    }
	}
    
}


