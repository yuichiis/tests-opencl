<?php

include('init_autoloader.php');


use Rindow\Math\Matrix\MatrixOperator;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\OpenCL;

$mo = new MatrixOperator();

//$mode = 'CPU-NORMAL';
//$mode = 'CPU-RAW';
$mode = 'GPU';
$size = 5000;
$epochs = 10;
$dtype = NDArray::float32;
//$dtype = NDArray::float64;


switch($mode) {
    case 'CPU-NORMAL': {
        $la = $mo->la();
        break;
    }
    case 'CPU-RAW': {
        $la = $mo->laRawMode();
        break;
    }
    case 'GPU': {
        $la = $mo->laAccelerated('clblast',['deviceType'=>OpenCL::CL_DEVICE_TYPE_GPU]);
        echo "blocking mode...\n";
        $la->blocking(true);
        //echo "non-blocking mode...\n";
        //$la->blocking(false);
        break;
    }
    default: {
        throw new Exception('Invalid mode');
    }
}
//$la = $mo->la();

$accel = $la->accelerated()?'GPU':'CPU';
echo "Mode:$accel($mode)\n";
$fp64 = $la->fp64()?'TRUE':'FALSE';
echo "Supports float64 on this device: $fp64\n";
if($dtype==NDArray::float64 && $fp64=='FALSE') {
    $dtype = NDArray::float32;
}

if($accel=='CPU') {
    $name = $la->getBlas()->getCorename();
    echo "CPU core name :$name\n";
    $theads = $la->getBlas()->getNumThreads();
    echo "CPU theads:$theads\n";
} else {
    $i = 0;
    $devices = $la->getContext()->getInfo(OpenCL::CL_CONTEXT_DEVICES);
    $name = $devices->getInfo(0,OpenCL::CL_DEVICE_NAME);
    echo "GPU device name :$name\n";
    $cu = $devices->getInfo($i,OpenCL::CL_DEVICE_MAX_COMPUTE_UNITS);
    echo "GPU Compute units :$cu\n";
    //$wg = $devices->getInfo($i,OpenCL::CL_DEVICE_MAX_WORK_GROUP_SIZE);
    //echo "GPU Max workgroup size :$wg\n";
}

$strdtype = $mo->dtypetostring($dtype);
echo "data type: $strdtype\n";
echo "computing size: [$size,$size]\n";
echo "epochs: $epochs\n";

$a = $mo->arange($size*$size,dtype:$dtype)->reshape([$size,$size]);
$b = $mo->arange($size*$size,dtype:$dtype)->reshape([$size,$size]);
$a = $la->array($a);
$b = $la->array($b);

// Dummy execution to compile OpenCL 
//$c = $la->gemm($a,$b);
//$la->finish();

$start = microtime(true);
for($i=0;$i<$epochs;$i++) {
    $c = $la->gemm($a,$b);
}
//$la->finish();
echo "elapsed time:".(microtime(true)-$start)."\n";
