<?php

namespace App\Presenters;

use Nette;
use App\Model\AllProjects;

class AllprojectsPresenter extends Nette\Application\UI\Presenter
{

    private $projectManager;

    public function __construct(AllProjects $projectManager)
    {
        $this->projectManager = $projectManager;
    }

    public function actionDefault()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

    }

    public function renderDefault()
    {
        $this->template->projects = $this->projectManager->getProjInfo($this->getUser()->id);
        $this->template->projects_own = $this->projectManager->getProjInfo2($this->getUser()->id);  
 
    }

}
