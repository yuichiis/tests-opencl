<?php

include('init_autoloader.php');

use Rindow\Math\Matrix\MatrixOperator;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\OpenCL;


function php2array($data,$a) {
    foreach($data as $i=>$row) {
        foreach($row as $j=>$d) {
            $a[$i][$j] = $d;
        }
    }
}

$mo = new MatrixOperator();

$dtype = NDArray::float32;
//$dtype = NDArray::float64;

$dataA = [[1,2],[3,4]];
$dataB = [[5,6],[7,8]];

$la = $mo->la();
$g_la = $mo->laAccelerated('clblast',['deviceType'=>OpenCL::CL_DEVICE_TYPE_GPU]);
$g_la->blocking(true);

$a = $la->alloc([2,2],dtype:$dtype);
$b = $la->alloc([2,2],dtype:$dtype);
php2array($dataA,$a);
php2array($dataB,$b);

$g_a = $g_la->array($a);
$g_b = $g_la->array($b);

$g_y = $g_la->gemm($g_a,$g_b);

$y = $g_la->toNDArray($g_y);

foreach($y as $row) {
    foreach($row as $d) {
        echo $d."\n";
    }
}


