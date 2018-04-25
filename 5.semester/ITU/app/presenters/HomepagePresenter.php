<?php

namespace App\Presenters;

use Nette;

class HomepagePresenter extends Nette\Application\UI\Presenter
{
	private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
    
    public function actionDefault()
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}else{
    		$this->redirect('Myprojects:default');
    	}
	}

    public function renderDefault()
	{
	}
}
