 $(document).ready(function(){
            $("h2").eq(0).addClass("active");
            $("article").eq(0).show();
            $("h2").click(function(){

            $(this).next("article").slideToggle("slow").siblings("article").slideUp("slow");
            $(this).toggleClass("active");
            $(this).siblings("h2").removeClass("active");
            });
        });