<?php

include('init_autoloader.php');

use Rindow\Math\Matrix\MatrixOperator;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\OpenCL;

$mo = new MatrixOperator();

$dtype = NDArray::float32;
//$dtype = NDArray::float64;

//// CPU
$la = $mo->la();

$a = $la->array([[1,2],[3,4]],dtype:$dtype);
$b = $la->array([[5,6],[7,8]],dtype:$dtype);
$c = $la->array([9,10],dtype:$dtype);

$y = $la->gemm($a,$b);  // y = matrix-matrix-multiply(a,b)
$z = $la->gemv($a,$c);  // z = matrix-vector-multiply(a,c)
$la->axpy($a,$b);       // b = a + b

echo "y=".$mo->toString($y)."\n";
echo "z=".$mo->toString($z)."\n";
echo "b=".$mo->toString($b)."\n";
print_r($y->toArray());

//// GPU
$la = $mo->laAccelerated('clblast',['deviceType'=>OpenCL::CL_DEVICE_TYPE_GPU]);
$la->blocking(true);

$a = $mo->la()->array([[1,2],[3,4]],dtype:$dtype);
$b = $mo->la()->array([[5,6],[7,8]],dtype:$dtype);
$c = $mo->la()->array([9,10],dtype:$dtype);
$a = $la->array($a);
$b = $la->array($b);
$c = $la->array($c);

$y = $la->gemm($a,$b);  // c = matrix-matrix-multiply(a,b)
$z = $la->gemv($a,$c);  // c = matrix-vector-multiply(a,c)
$la->axpy($a,$b);       // b = a + b

echo "y=".$mo->toString($y)."\n";
echo "z=".$mo->toString($z)."\n";
echo "b=".$mo->toString($b)."\n";

print_r($y->toArray());
