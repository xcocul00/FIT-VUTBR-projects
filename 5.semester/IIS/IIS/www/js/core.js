
$(document).ready(function(){
	var dateToday = new Date();

	$('#userarea').click(function(event){
		$('#login_form').slideToggle(400);
	});

    $.datepicker.regional['cs'] = {
	    closeText: 'Zatvoriť',
	    prevText: '&#x3c;Skôr',
	    nextText: 'Neskôr&#x3e;',
	    currentText: 'Teraz',
	    monthNames: ['január', 'február', 'marec', 'apríl', 'máj', 'jún', 'júl', 'august',
	      'september', 'október', 'november', 'december'
	    ],
	    monthNamesShort: ['jan', 'feb', 'mar', 'apr', 'máj', 'jún', 'júl', 'aug', 'sep', 'okt', 'nov', 'dec'],
	    dayNames: ['nedeľa', 'pondelok', 'utorok', 'streda', 'štvrtok', 'piatok', 'sobota'],
	    dayNamesShort: ['ne', 'po', 'út', 'st', 'št', 'pi', 'so'],
	    dayNamesMin: ['ne', 'po', 'út', 'st', 'št', 'pi', 'so'],
	    weekHeader: 'Týž',
	    dateFormat: 'dd/mm/yy',
	    firstDay: 1,
	    isRTL: false,
	    showMonthAfterYear: false,
	    yearSuffix: ''
  	};

    $.datepicker.setDefaults($.datepicker.regional['cs']);

    $("#datepicker_from1").datepicker({
   	});

    $("#datepicker_to1").datepicker({ 	
   	});

    $("#datepicker_from").datepicker({
    	minDate:dateToday,
    	onSelect: function(date, obj){
        	$('#date_input').val(date);
        	$('#datepicker_from_div').text("Dátum od: "+ date);
        }
   	});

    $("#datepicker_to").datepicker({
    	minDate:dateToday,
    	 defaultDate: "+1w",
    	 onSelect: function(date, obj){
        $('#date_output').val(date);
        $('#datepicker_to_div').text("Dátum od: "+date);
   	}});

  
	$('#datepicker_from_div').text("Dátum od: "+ $('#datepicker_from').datepicker( { dateFormat: 'dd,MM,yyyy' } ).val());
	$('#datepicker_to_div').text("Dátum od: "+ $('#datepicker_to').datepicker( { dateFormat: 'dd,MM,yyyy'} ).val());

	$('.show_register').click(function(event){
		$('.register_user').slideToggle(400);
	});

});


	
