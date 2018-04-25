<?php

namespace App\Presenters;

use Nette\Application\UI\Control;
use Nette\Application\UI\Form;

class PollControl extends Control
{
  public function render($var)
    {
        $template = $this->template;
        $template->setFile(__DIR__ . '/poll.latte');
        // vložíme do šablony nějaké parametry
       
        // a vykreslíme ji
        $template->render();

  
    }

    protected function createComponentRegister(){
                  $form = new Form;

    $form->addText('name', 'Jméno:')
        ->setRequired();

    $form->addEmail('email', 'Email:');

    $form->addTextArea('content', 'Komentář:')
        ->setRequired();

    $form->addSubmit('send', 'Publikovat komentář');

        return $form;
    }
}
