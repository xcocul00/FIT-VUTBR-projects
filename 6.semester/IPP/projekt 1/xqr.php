<?php
#XQR:xcocul00
#XQR:xcocul00

/*
 * author: Maros Cocula
 * subject:  IPP
 * project:  proj1
 * variant: XML Query in PHP5
 */
  error_reporting(E_ERROR | E_PARSE);

/*
  Error code enum
*/
  abstract class code_error{

    const OK = 0;
    const error_param = 1;
    const error_read_file = 2; 
    const error_write_file = 3;
    const error_wrong_input = 4;
    const error_query = 80;
    /*10-99 other for XQR
     100-127 other
     */
  }


/*
  load parameters
*/

  $opt=load_parameters($argv);

// open file, if parameter -input is entered

  if(isset($opt["--input"]))
    {
      if(is_link($opt["--input"]))
      {
        $opt["--input"]=readlink($opt["--input"]);
      }
      $opt["--input"]=realpath($opt["--input"]);
      $xml=load_file($opt["--input"]);
    }
  else
    {
      $xml=file_get_contents("php://stdin");
    }

// check if the XML format of data is right

  try
  {
    $xml=new SimpleXMLElement($xml);
  }
  catch(Exception $e)
  {
    exit_program("Bad XML format, try to use -help \n",code_error::error_wrong_input);
  }

  $xml_write="";

// parse query
  if(isset($opt["--query"]))
  {
    $query=parse_query($opt["--query"]); 
  }

  if(isset($opt["--qf"]))
  {
    if(is_link($opt["--qf"]))
    {
      $opt["--qf"]=readlink($opt["--qf"]);
    }
    $opt["--qf"]=realpath($opt["--qf"]);
    $query=parse_query(load_file($opt["--qf"]));
  }

  if(empty($query['FROM'][0])&& empty($query['FROM'][1]))
  {
    $query_result=array();
  }
  else
  {
    $query_result=run($query,$xml);
  }
  if(!empty($query_result))
  {
    foreach ($query_result as $item)
      $xml_write=$xml_write.$item->asXML();
  }
// write root to XML file if is set parameter -root=element

  if(isset($opt["--root"]))
  {
    if(empty($xml_write))
      $xml_write="<".$opt["--root"]."/>";
    else
      $xml_write="<{$opt["--root"]}>"."$xml_write"."</{$opt["--root"]}>";
  }

// write header to XML file if parameter -n is not set
  if(!isset($opt["-n"]))
  {
    $xml_write = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"."$xml_write";
  }

// write final file
  if(isset($opt["--output"]))
   {

      if(is_link($opt["--output"]))
      {

        $opt["--output"]=readlink($opt["--output"]);
      }
      if(file_exists($opt["--output"])){
        $opt["--output"]=realpath($opt["--output"]);
      }
      write_file($opt["--output"],$xml_write);
   }
   else
    print $xml_write;
  
/*
  function that parse parameters from command line, check and return array with parameters
  input -> array $argv
  output -> array with parameters 
*/
function load_parameters($argv)
{

  // array with matching regular expressions
    $params [0] = "/^.*\.php$/";
    $params [1] = "/^(-n)$/";
    $params [2] = "/^--input=(.*)$/";
    $params [3] = "/^--output=(.*)$/";
    $params [4] = "/^--qf=(.*)$/";
    $params [5] = "/^--query=(.*)$/";
    $params [6] = "/^--root=(.*)$/";
    $params [7] = "/^(--help)$/";
    $params [8] = "/^(-help)$/";
    $params [9] = "/^-input=(.*)$/";
    $params [10] = "/^-output=(.*)$/";
    $params [11] = "/^-qf=(.*)$/";
    $params [12] = "/^-query=(.*)$/";
    $params [13] = "/^-root=(.*)$/";

    // syntax check the validity of arguments 
    $counter=-1;
    foreach ($argv as $argument) 
    {
      ++$counter;
        for ($i = 0; $i < count($params); $i++) 
        { 
            if (preg_match($params[$i], $argument))
            {
              
              if($i>7 && $i<14)
              {   
                // from short option do long option of argument (from -xyz to --xyz)        
                $argv[$counter]="-".$argv[$counter];
              }
              break;
            }
            else
            {
              if ($i == 13)
              {
                exit_program("Bad parameters, try to use -help or --help\n",code_error::error_param);
              }
            }            
        }
    }

    // parameter check
  parse_str(implode('&', array_slice($argv,1)),$opt);
    if ((array_key_exists("--help", $opt) and (count($argv) == 2)))
        Help();
    if ((count($argv)-1 != count($opt)) 
    or (isset($opt["--query"]) and isset($opt["--qf"]))
    or (!isset($opt["--query"]) and !isset($opt["--qf"]))
    or (array_key_exists("--help", $opt) and (count($argv) != 2)))
        {
        exit_program("Bad parameters, try to use -help or --help\n",code_error::error_param);
        }
    return $opt;
}

/*
  function that parse query
  input -> query from parameter --qf or --query
  output -> array wwith parsed query named $parsed
*/
function parse_query($query){
  $patern='/^SELECT\s+\w+\s+FROM(\s+(\w+|\w+\.\w+|\.\w+))?(\s+WHERE\s+((NOT\s+)?)*((\w+)|(\w+\.\w+)|(\.\w+))(\s+)*(=|>|<|CONTAINS)(\s+)*((\"\w+\")|(\d+)))?(\s+)?(\s+LIMIT\s+\d+)?(\s+)?$/';
  $constants = array('SELECT','FROM', 'LIMIT','WHERE','ROOT','NOT');

  if(! preg_match($patern, $query))
  {
      exit_program("Bad query, try to use -help or --help\n",code_error::error_query);
  }

  // SELECT

  preg_match('/SELECT\s+(\w+)\s/', $query, $parsed['SELECT']);
  $parsed['SELECT']=$parsed['SELECT'][1];
  if((in_array($parsed['SELECT'], $constants)) | (is_numeric($parsed['SELECT'])))
  {
    exit_program("Bad query (SELECT), try to use -help or --help\n",code_error::error_query);
  }

  // FROM  
  preg_match('/FROM(\s*)(\w+|\w+\.\w+|\.\w+)?($|\s+)/', $query, $parsed['FROM']);
  $parsed['FROM'] = preg_split("/\./",$parsed['FROM'][2]);
  if(preg_match('/FROM\s+WHERE\s+WHERE\s+/', $query) | (is_numeric($parsed['FROM'])))
  {
    exit_program("Bad query (FROM), try to use -help or --help\n",code_error::error_query);
  }
  if((in_array($parsed['FROM'], $constants)) && ($parsed['FROM'] != "ROOT"))
  {
    if($parsed['FROM']=="WHERE")
    {
      $parsed['FROM']="";
    }
    else
    {
      exit_program("Bad query (FROM), try to use -help or --help\n",code_error::error_query);
    }
  } 

  // WHERE
  
  if(strpos($query,'WHERE'))
  {
    if(preg_match('/WHERE\s+(((NOT\s+)?)*((\w+)|(\w+\.\w+)|(\.\w+))(\s+)*(=|>|<|CONTAINS)(\s+)*((\"\w+\")|(\d+)))($|\s)/', $query, $parsed['WHERE']) == 0)
      {
        exit_program("Bad query (WHERE), try to use -help or --help\n",code_error::error_query);
      }
    $parsed['WHERE']=$parsed['WHERE'][1];
    // REMOVE NOT
    $parsed['WHERE']=str_replace("NOT ", "", $parsed['WHERE'], $not_clause);
    preg_match('/(\w+|\w+\.\w+|\.\w+)\s*?(=|>|<|CONTAINS)\s*?(\"\w+\"|\d+)/', $parsed['WHERE'],$clause);
    $parsed['WHERE']=array();
    // OPERATOR FOR WHERE
    $parsed['WHERE'][2]=$clause[2];
    // VALUE OF LITERAL
    if(substr($clause[3], 0,1) == '"' && substr($clause[3], -1) == '"')
    {
        $clause[3] = substr($clause[3], 1, -1);
    }
    else
        $clause[3] = (double)$clause[3];
    $parsed['WHERE'][3]=$clause[3];
    // VALUE OF ATTRIBUTTE
    $clause=preg_split("/[.]/", $clause[1]);
    $parsed['WHERE'][0]=$clause[0];
    if(isset($clause[1]))
    {
      $parsed['WHERE'][1]=$clause[1];
      if(is_numeric($parsed['WHERE'][1][0]))
      {
      exit_program("Bad query (WHERE), try to use -help or --help\n",code_error::error_query);
      }
    }
    else
      $parsed['WHERE'][1]="";

    // NOT
    if(($not_clause%2)==0)
    {
      $parsed['WHERE'][4]=false;
    }
    else
      $parsed['WHERE'][4]=true;

    if(($parsed['WHERE'][2] == "CONTAINS")&&(gettype($parsed['WHERE'][3])!="string"))
    {
      exit_program("Bad query (WHERE), try to use -help or --help\n",code_error::error_query);
    }
  }
  // LIMIT
  if(strpos($query,'LIMIT'))
  {
    preg_match('/LIMIT\s(\d+)(\s+)?$/', $query, $parsed['LIMIT']);
    $parsed['LIMIT']=(int)$parsed['LIMIT'][1];
  }
  return $parsed;
} 

/*
  function that run query and give result
  input -> parsed query in array and input file in 
  SimpleXMLElement object
  output -> array with result
*/
function run ($query,$xml)
{
  // GET AN ELEMENT
  if ($query['FROM'][0]=="ROOT" || $query['FROM'][0]=="")
  {
    $path=NULL;
  } 
  else
  {
    $path=$query['FROM'][0];
  }
  // GET AN ATTRIBUTTE
  if(!empty($query['FROM'][1]))
  {
    $path=$path.".{$query["FROM"][1]}";
  }
  $xml=my_xpath($path,$xml);

  if(!empty($xml))
  {
    $xml=$xml[0];
  }
  else
    return (string) NULL;

  // SELECT ELEMENT from FROM
  $path=$query['SELECT'];
  $xml=my_xpath($path,$xml);

  if(empty($xml))
  {
    return (string)NULL;
  }
  else
  {
    if(empty($query['WHERE']))
    {
    $result=$xml;
    }
    else
    {
      // WHERE
      if(empty($query['WHERE'][0]))
      {
        $path=NULL;
      }
      else
      {
        $path=$query['WHERE'][0];
      }

      if(!empty($query['WHERE'][1]))
      {
        $path=$path.".{$query["WHERE"][1]}";
      }
      $result=array();
      
      foreach($xml as $element)
      {
        $flag=0;
        $validity=false;

        if(!empty($query['WHERE'][1]))
        {
          $tmp_array=array_keys((array)$element);
          foreach ($tmp_array as $key) {
            if(strcmp($key, "@attributes")==0)
            {
              $data=(array)$element;
              $data=$data["@attributes"];
              if(array_key_exists($query['WHERE'][1], $data))
              {
                $flag=1;
                $part_item=$data[$query['WHERE'][1]];
              }
            }
          }
          if(isset($query['WHERE'][0]) && $query['WHERE'][0] != "")
          {
            if(strcmp($query['WHERE'][0], $query['SELECT'])!=0)
            {
              $flag=0;
            }
          }
        }
        if($flag)
        {
          $validity=compare_operator($query,$part_item);
        }
        else
        {
          $final=my_xpath($path,$element);
          foreach ($final as $part_item) {
            $validity=compare_operator($query,$part_item);
            break;
          }
        }
        if($query['WHERE'][4]==true)
        {
          $validity=!$validity;
        }
        if(empty($query['WHERE'][1]))
        {
          $tmp_array=array_keys((array)$part_item);
          foreach ($tmp_array as $key) {
            if(!strcmp($key, "@attributes") && !is_int($key))
            {
              $validity=false;
              break;
            }
          }
        }
        if($validity)
        {
          array_push($result, $element);
        }
      }
    }
  }
  if(empty($result))
  {
    return (string)NULL;
  }
  if(isset($query['LIMIT']))
  {
    $result=array_slice($result, 0, $query['LIMIT']);
  }
  return $result;
}

/*
  function that return result from compare
  input -> parsed query in array and item to compare
  output -> value true or false
*/
function compare_operator($query,$item)
{
  if($query['WHERE'][2]=="CONTAINS")
  {
    if(strpos($item,$query['WHERE'][3]) !== false)
    {
      return true;
    }
  }
  else if ($query['WHERE'][2]=="=")
  {
    if((is_double($query['WHERE'][3]) && ((double)$item == $query['WHERE'][3])) || strcmp($item, $query['WHERE'][3])==0)
    {
      return true;
    }
  }
  else if ($query['WHERE'][2]=="<")
  {
    if((is_double($query['WHERE'][3]) && ((double)$item < $query['WHERE'][3])) || strcmp($item, $query['WHERE'][3]) < 0)
    {
      return true;
    }
  }
  else if ($query['WHERE'][2]==">")
  {
    if((is_double($query['WHERE'][3]) && ((double)$item > $query['WHERE'][3])) || strcmp($item, $query['WHERE'][3]) > 0)
    {
      return true;
    }
  }
  return false;
}

/*
  function that return result after xpath query
  input -> attribute to find and input data
  output -> array of result data
*/
function my_xpath($cmd,$xml)
{
  if(($cmd===NULL) ||(!strcmp($cmd,$xml->getName())))
  {
    return $xml;
  }
  $keywords=preg_split("/\./", $cmd);
  if($keywords[0]!=NULL)
  {
    $tmp_array=array();
    $tmp_array=find_path($keywords[0],$xml,$tmp_array);
  }
  else
  {
    $tmp_array=$xml;
    if(isset($keywords[1]))
      {
       $tmp_array=find_attribute($keywords[1],$tmp_array,0);
      }
    return $tmp_array;
  }
  if(isset($keywords[1]))
  {
    $tmp_array=find_attribute($keywords[1],$tmp_array,1);
  }
  return $tmp_array;
}

/*
  function that return result after xpath query for element
  input -> attribute to find,input data and result array
  output -> array of result data
*/
function find_path($cmd,$xml,$tmp_array)
{  
  foreach ($xml as $key) 
  {
    if(strcmp($key->getName(), $cmd))
    {
      $tmp_array=find_path($cmd,$key,$tmp_array);
    }
    else
    {
      array_push($tmp_array, $key);
    }
  }
  return $tmp_array;
}

/*
  function that return result after xpath query for attribute
  input -> attribute to find,input data and flag
  output -> array of result data
*/
function find_attribute($cmd,$xml,$flag)
{
  $tmp_array=array();
  if($flag)
  {
    foreach ($xml as $key ) 
    {
      if(strcmp($key->attributes(),NULL))
        {
          $tmp=array_keys((array)$key);
          if(array_key_exists('@attributes', $key))
          {
            $data=(array)$key;
            $data=$data["@attributes"];
            if(array_key_exists($cmd, $data))
            {
              array_push($tmp_array, $key);
            }
          }
        } 
    }
  }
  else
  {
    foreach ($xml as $key) 
    {
        if(array_key_exists('@attributes', $key))
        {
          $data=(array)$key;
          $data=$data["@attributes"];
          if(array_key_exists($cmd, $data))
          {
            array_push($tmp_array, $key);
          }
        }
        else
        {
          $tmp_array=find_attribute($cmd,$key,$flag);
        }
    }
  }
  return $tmp_array;
}

/*
  function that displays help message on display
  input -> none
  output -> displayed message and program end after print
*/
function Help()
{
    echo (
      "------------------------------------------------------------------------------\n".
       " --help                 - Show help\n".
       " --input=filename.ext   - Input file with xml\n".
       " --output=filename.ext  - Output file with xml\n".
       " --query='query'        - Query under xml - can not be used with -qf attribute\n".
       " --qf=filename.ext      - Filename with query under xml\n".
       " -n                     - Xml will be generated without XML header\n".
       " -root=element          - Name of root element\n".
      "------------------------------------------------------------------------------\n"
    );
    exit(code_error::OK);
}
/*
  function that open input file
  input -> argument from --input
  output -> valuable with input data
*/
function load_file($opt)
{
  if(file_exists($opt))
  {
    if (is_readable($opt))
    {
      $handle = fopen($opt, "r");
            $contents = fread($handle, filesize($opt));
            fclose($handle);
            return $contents;
    }
    else
    {
      exit_program("Cannot open file, try to use --help\n",code_error::error_read_file);
    }
  }
  else
  {
    exit_program("File does not exist, try to use --help\n",code_error::error_read_file);
  }
}

/*
  function that write to output file
  input -> argument from --output and valuable with result
  output -> array of result data
*/
function write_file($opt, $contents)
{
      if (is_writable($opt)) 
      {
          if (!$open = fopen($opt, "w")) 
          {
              exit_program("Cannot write to file\n", code_error::error_write_file);
          }
      }
      else 
      {
          $open = fopen($opt, "x");
      }
      fwrite($open, $contents);
      fclose($open);
    return;

}

/*
  function that end program and print error code
  input -> stderr message and error code (may be 0)
  output -> end of program, write error code to stderr
*/
function exit_program($message, $code)
{
  fwrite(fopen('php://stderr', 'w+'), $message);
  exit($code);
}
?>