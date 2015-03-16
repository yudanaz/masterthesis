[33mcommit 02891a74c6735b72b2afac23c010f7b758301eb9[m
Author: maurice auf hp latop2 <yudanaz@gmail.com>
Date:   Fri Mar 13 17:00:57 2015 +0100

    CAPTURE: some laptop-related changes.

[33mcommit 6990632c9aea39d53eb8fda575a324d249cea679[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 12 17:32:22 2015 +0100

    PREPROC: disparity improve by seed superpixels implemented

[33mcommit 0d11b794fe89174e0a229f47ff479e93a32279eb[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 12 15:18:14 2015 +0100

    CAPTURE: usb port now set in settings file

[33mcommit a1a4857caa37aa98090afaf4728c8b738463af42[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 12 14:50:57 2015 +0100

    CAPTURE: USB port for flashlight can be chosen from GUI

[33mcommit 8b2dd89550fe0394794e04b20fd82d8d1128eed4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 12 14:18:38 2015 +0100

    CAPTURE: solved small problem with sound files

[33mcommit 9946dcd6645f060d3ec5fe1b66bafd9374211311[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 18:12:19 2015 +0100

    coisinhas

[33mcommit d4027df5dcd0397f2e5008a385fdbe4892c340a8[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 16:27:48 2015 +0100

    CAPTURE: drawing target cross for camera positioning

[33mcommit 8c6cf7dc4724d61bc2bff84eb36b6b653157cfd3[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 15:23:30 2015 +0100

    PREPROC: images are cropped according to minimal inner box for registered RGB and remapped/rectified kinect depth

[33mcommit f840511c649ff75cd177c40f11c96e9af43672dd[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 15:12:31 2015 +0100

    PREPROC: crop by kinect depth workinggit stage -A!

[33mcommit 62bb3f28183f31a3ae7723fbcc04f929145da637[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 14:15:19 2015 +0100

    PREPROC: improved rgb-nir mapping by horizontal shift with median distance comp.

[33mcommit baaa1594ad85c46fa60dbcfdde442ea6d3f67a32[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Mar 11 14:00:14 2015 +0100

    PREPROC: kinect depth remap during 3d-2d projection

[33mcommit 47193d409dbee7320ac90e7a4326c59105c498af[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Mar 10 13:08:49 2015 +0100

    PREPROC: crop rectangle from kinect depth. Problem/TODO: Must get corner points of reprojected depth map AFTER undistortion.

[33mcommit 29968665541def2d58275f90cd30e7c6a7b913d7[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Mar 10 12:08:43 2015 +0100

    PREPROC: final crop implemented, based on registered RGB. HOG block and cell parameters adjustable. CSStereo on original sized image.

[33mcommit 9960586749958a090d997cbe57f1dadc0eb0de62[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Mar 9 16:27:52 2015 +0100

    kinect depth inverted

[33mcommit 7b3e6d737c56f23691d4384f79e3fb0f4211ebc2[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Mar 9 14:52:25 2015 +0100

    PREPROC: CSStereo: SIMD support activated in SGM Wrapper

[33mcommit d3f31396d6a99477d79988286e5d5283a839cc10[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Mar 9 12:39:59 2015 +0100

    PREPROC: kinect depth holes filled, reprojection error solved.

[33mcommit 679f5e970204ef8aebebbd3a5d4823b07907db41[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Mar 6 17:01:21 2015 +0100

    PREPROC: saving proj matrix for ir->nir, need vertical shift for correct reprojection

[33mcommit 8d5bcd0dec2f5f9c8f22006652d0349ad0b6d2c4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Mar 6 15:15:09 2015 +0100

    PREPROC: RGB->NIR registration using HOG desr. and different registration methods (thin plate spline, warp) .

[33mcommit 6e895045f85284b691129768f04451f632e8d89a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Mar 6 15:14:00 2015 +0100

    PREPROC: RGB->NIR registration using HOG desr. and different registration methods (thin plate spline, warp) .

[33mcommit a6104cef96e8934191b771d1ae845a656e47c1ca[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Mar 6 10:59:00 2015 +0100

    PREPROC: fix depth map with simple shadow assumption. Tested Burrus' 3d->2d reprojection. Does exactly the same thing as opencv projectPoints()

[33mcommit d0db0f8b9af41613b078a89b02f285172815ec33[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Mar 6 10:58:08 2015 +0100

    PREPROC: fix depth map with simple shadow assumption. Tested Burrus' 3d->2d reprojection. Does exactly the same thing as opencv projectPoints()

[33mcommit a88b2eb1bb99442f23f32040ae099903884983e1[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 5 17:06:14 2015 +0100

    working on: HOG_crossSpectralStereoMatcher::setBestDescriptors

[33mcommit 4a53c582022f9a0acb50b6bd8cd8142fb9b1dcb7[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 5 15:44:46 2015 +0100

    CALIB: Now mapping all to NIR instead of RGB. NIR is left in stereo rig. Separate calib for IR->NIR, RGB->NIR. NIR img is now 3-channel png.

[33mcommit 792ee52de12711331055380cb32bb019fdaa5a9f[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 5 15:43:16 2015 +0100

    CALIB: Now mapping all to NIR instead of RGB. NIR is left in stereo rig. Separate calib for IR->NIR, RGB->NIR. NIR img is now 3-channel png.

[33mcommit 2914cd40e7147863dbab7152abf4bfa07cf86ba2[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Mar 5 10:10:37 2015 +0100

    RBDNIR capture: refactoring, some GUI improvements

[33mcommit 785b89b09570263d91069d4ac1d397b1c06eafc6[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Mar 3 13:55:44 2015 +0100

    RGBDNIR capture: depth as 8bit, some small changes.

[33mcommit 281c6ecdff540d726b4b7512bc4995178cb79401[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Mar 3 12:40:18 2015 +0100

    RGBDNIR capture: new ringflash working

[33mcommit 18408ba1858fa0f631b35ee7db47df8a43dcac3c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Sun Mar 1 20:19:16 2015 +0100

    CAFFE: some bugs found, problem still out there.. :(

[33mcommit e04a92f0715949ea1b4528fc57150aacd9934b7e[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 26 18:30:20 2015 +0100

    CAFFE: fixed uniform subpatch selection

[33mcommit d0f8a54f9d2a5144cb57eac3ba6550a66ef1cfc9[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 26 13:00:07 2015 +0100

    CAFFE uniform subpatching instead of random patches and other improvements

[33mcommit 47fe87709b9b8368b332b25ed44ad9d76caa2e88[m
Merge: ce1836f 7343939
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 25 12:06:54 2015 +0100

    Merge branch 'master' of https://github.com/yudanaz/masterthesis
    
    Conflicts:
    	RGB_NIR_Depth_capture/RGB_NIR_Depth_capture.pro.user
    	SegmentationHelper/SegmentationHelper.pro.user
    	SegmentationPreprocessor/SegmentationPreprocessor.pro.user
    
    merge lab home resolved

[33mcommit ce1836fd518613244f5d1736a879078217e2d820[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 25 11:48:14 2015 +0100

    CAFFE: image type (extension e.g. png) as net parameter

[33mcommit 8719e495b9aeb6329810831d9bb2e98784fe0842[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 24 16:33:59 2015 +0100

    cam and camRig calib with RGB, NIR & IR works. preprocessing problem with kinect depth - no image

[33mcommit 116b9e95d25f42939a692ef34379695485bf547a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 24 16:00:38 2015 +0100

    cross-Spectr.StMatching with HOG-SGM läuft!

[33mcommit 36adf4f49972236196619fc0fa834886975e363c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 24 13:49:16 2015 +0100

    parametrization of SGM and WTA optimization via GUI - ok!

[33mcommit be7348e038379be958be8a3dd6926e7ec5bf6624[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 24 12:29:39 2015 +0100

    Stereo Matching parameter window, distance threshold for WTA optimization.

[33mcommit 734393925df1f809ecb176c65901d7a02715e036[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 18 15:42:15 2015 +0100

    RGBDNIR capture: RGB calib simulation OK. mirror images for convenience. More key events. Debugging.

[33mcommit a99fa17e5e9f73dc2b64e54557f49c6aad036f22[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 17 17:15:58 2015 +0100

    Cross-Spectral Stereo with HOG and SGM somehow works, but still not good. Have to check different SGM params...?

[33mcommit f20e4db40b92622a770d5c22ba86d86b4978b288[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 17 10:58:30 2015 +0100

    not much

[33mcommit 4f3cc589a50faec1870f6ddda78c0d98a445afb9[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Feb 16 18:15:40 2015 +0100

    Disparity with HOG works with WTA strategy, SGM re-adapted from opencv for extension of cost function

[33mcommit bb676e671da4d7fd54070d14bdfcc4a4343db380[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Feb 13 17:16:30 2015 +0100

    verfickte Dreckscheisse den ganzen Tag versucht und HOG funktioniert nicht :(

[33mcommit 6ca376ea8de3bb03e52c40378ccc672c14a7a914[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 12 18:22:03 2015 +0100

    ImgPreProc: HOG CrossSpectral Stereo, somehow working, but still several issues.

[33mcommit ce19cb612d2dfc6fb022c7ec18acc5dc8102cc59[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 12 12:14:06 2015 +0100

    ImgPreProc: CrossSpectral Stereo through localized contrast normalization tested & partially implemented. some refactoring.

[33mcommit 1d13c64fdb151c160eaecb4992903f759442ba64[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 11 18:17:15 2015 +0100

    RGBDNIR capture: loads RGB cam intrinsics resize/crop factor and simulates RGB image after calibration

[33mcommit 7ff86d3d4d95d583ae8ff7ea24d0f999acb575f9[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 11 17:34:42 2015 +0100

    Seg.PreProc.: Cam and Cam-Rig Calibration working. Results more/less ok (Chessboard images bad?)

[33mcommit 398aa78ff999ee58ed2b66145d5f69b4055d141b[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 11 12:48:37 2015 +0100

    Seg.PreProc.: Error handling for unfound chessboard corners. Informative ProgressDialogs. App now responsive all the times.

[33mcommit 9414fdbdca24a395b7dbd91a33c8f6e223466eb3[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 10 16:07:34 2015 +0100

    ImagePreprocessor: pipelines total ueberarbeitet, alles bereit fuer multispectral stereo matching

[33mcommit 7fb4691cc69040905ae10f89ce3b941a623319cd[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Feb 10 13:08:59 2015 +0100

    ImagePreprocessor: separate calibration for cameras and cam rig.

[33mcommit 4492c7e7156b130c52a56dcee1c9a81c0e74fe02[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Feb 9 16:10:32 2015 +0100

    RGBDNIR capture - Menu: choose which channels to save and show. Toggle multichannel (faster without, better for calib imgs)

[33mcommit 7b56b8e948cf94ffce212f06e4c1263c2f573635[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Feb 9 14:06:16 2015 +0100

    cams work with PoE-switch (MTU must be at <= 1500)

[33mcommit 5801f54f876503ba338c40e25fcb7ccbcf00427f[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Feb 6 17:34:01 2015 +0100

    RGBDNIR capture: Saving Images in own Thread

[33mcommit 11f463aad189738be46a069fc3804fcdef1a396b[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Feb 6 15:57:14 2015 +0100

    RGBDNIR capture: small change in mutex

[33mcommit ad201f85a5e651b91848a9a1407f3388707a6db4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Feb 6 15:45:51 2015 +0100

    RGBDNIR capture: solved camera problem (network package collisio) with mutex. Also: Prosilica exposure config.

[33mcommit 0989b3c6d7a537cc702a8be4becf551e39188d97[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 5 17:32:15 2015 +0100

    Cross-Bilateral-Filter of Paris eingebunden

[33mcommit 8e69150db36503a98903bce2c6d591c00c0e8e7c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 5 13:54:36 2015 +0100

    ImagePreProcessor - some changes

[33mcommit 0f5150a14ad903971269525624dd93f23e63135e[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Feb 5 12:32:21 2015 +0100

    reprojection from depth map to rgb image is more or less working

[33mcommit 1bbf2b9a39d4b7cf4172907d2b22ca3bc175a37b[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Feb 4 17:25:37 2015 +0100

    Depth-to-RGB projection weiter. Noch nicht fertig

[33mcommit 45738356d9c839f032d1c7e7627e09454fea3198[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 30 16:49:08 2015 +0100

    RGBDNIR capture improvements

[33mcommit 51dc7f8a83c519b905047e253f8e6afba66ae297[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 29 18:03:02 2015 +0100

    Started Segmentation Preprocessor. Next step: Map from Depth->RGB

[33mcommit ea1c65c071538063bf629f9eb918a4fbeb50094a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 29 18:01:41 2015 +0100

    Started Segmentation Preprocessor. Next step: Map from Depth->RGB

[33mcommit 7047cae4ea2ab753219fd016e9c6bb8512751d34[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 28 16:29:17 2015 +0100

    this and that

[33mcommit 7bff23ec87cf648953cd9047f72547d568816da7[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 28 12:12:07 2015 +0100

    RGBDNIR capture: IR capture for calibration is working, altough slow

[33mcommit 941c5bd08eb76a59d63a4399f7dfb5145d0a9e2d[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 27 17:20:11 2015 +0100

    getting IR image from kinect. still buggy and slow

[33mcommit 9d36ff9d3b48ed870cd8136359ea2531ab3766a5[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 27 13:30:39 2015 +0100

    RGBDNIR-capture: unique image names, improved auto-timer

[33mcommit 2ff084a27647aa15349f25a5b671fd34d7f3b039[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Jan 26 18:35:43 2015 +0100

    minor changes

[33mcommit 42b821255384fef3ceab74efb85c789fcec911ec[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Jan 26 15:05:08 2015 +0100

    CNN debugged + select n random pixels in each image + outputs loss log.

[33mcommit 73ab34a63b1cd8db76d5deadcebd6b34677959a5[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 23 15:28:28 2015 +0100

    make Test val -> debugged

[33mcommit c98b8d70e3430f2107cc80640c3218b959c65793[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 23 15:16:33 2015 +0100

    CutImageIn4Pieces, make testVal

[33mcommit 7e5720da92137204933496c135ba043be0b5886e[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 23 13:45:44 2015 +0100

    CAFFE bug found, Net is learning with all RGBDNIR and multiscale!

[33mcommit f084237a592dfef1c124aedbdce6d1f63ed1e3b6[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 22 10:39:34 2015 +0100

    improved CutImageIntoPieces, minor changes in CAFFE_Dev

[33mcommit 5bd75f7f57cb93dd5ad366aef9b59cd3c93367b9[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 21 18:50:53 2015 +0100

    SegmentationHelper: cutImageInPieces und make Train und Test text files

[33mcommit a8c8e5d479bf74eced1007ae9b7e130178bd8e88[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 21 17:25:15 2015 +0100

    CAFFE_Dev RGBDNIR Anpassung kompiliert! :)

[33mcommit a6a3df9fabfdda4642f16f873eb3b989d24deb62[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 21 12:16:09 2015 +0100

    SolverRGBDNIR implemented, still not compiling

[33mcommit 9acbcb9211e7d37c62069036c62934598bc62576[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 20 14:03:15 2015 +0100

    CAFFE-Erweiterung: Net_RGBDNIR implementiert: Einlesen von Bildern direkt über memory in MemoryDataLayer.

[33mcommit f5bfa7fd830efaf8b20746496ba910c9b0c43016[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 16 16:24:49 2015 +0100

    changed data preprocessing2

[33mcommit e20c1a649d22eb11cda4a122a3eba2850bf21eab[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 16 15:23:55 2015 +0100

    changed data preprocessing - all ROIS in big image now, per channel. Much faster.

[33mcommit c84cb50d8635a7f11f47681ac92fd5cc313eb9ce[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 15 16:20:45 2015 +0100

    downsampling verbessert: label image ohne smoothing und resize statt pyrDown bei images

[33mcommit 8f8ef12c3ed0a6e03a6b23622beae4056baedd6f[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 15 14:07:53 2015 +0100

    scale pyramid implemented

[33mcommit 2f1e5462610b741658a65f4328821e8a9c0137ab[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 15 09:05:45 2015 +0100

    improved: concat label files into train and val label files by percentage

[33mcommit f7c38d8a999a07813f5f64a795550446dc069311[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 14 16:45:01 2015 +0100

    make patches - changes. speed improvement etc. all channels saved in same image for the sake of speed and space

[33mcommit 96599754b7875edca2a82dcb5618f4b4d71d8a84[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 14 12:36:50 2015 +0100

    make patches als Button, etc.

[33mcommit 627aebaca47a10e66169352e0ac4e6e46f9a2267[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 14 11:06:36 2015 +0100

    transformed stanford bckgrnd dataset label txt-files in png-files

[33mcommit 7ef6c48bbe3637e59740edf0333ad3870140aa1d[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 14 10:23:50 2015 +0100

    make image patches: creating label-file and bugfixes

[33mcommit 98780a405a367278b62efdb01aaa90bb45eb4278[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 13 19:50:54 2015 +0100

    images (separate channels) saved as tar arquive

[33mcommit 9f90b882e42f26cb39c4edb3e55575999774292b[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 13 18:53:46 2015 +0100

    image patching workig. channels are saved in separate files.

[33mcommit 49d31afd5a4a846d35914e9bcea3486c34e169f0[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 13 14:52:09 2015 +0100

    image patching implemented. saving as ppm - still very large...

[33mcommit be398dd60187d8d3de542e75e7b111eb8a5d0dd4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Jan 12 17:02:30 2015 +0100

    Local Normalization implemented in Helper.h class

[33mcommit 42c7a41463ad6b280ad19f3f6c85bba4a5dfbc4a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 9 14:06:04 2015 +0100

    ..caputre timed capture series implemented.

[33mcommit 0df5e62e2a41e444b53d59488b58bc5f0b12a6c6[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 9 13:03:04 2015 +0100

    ..Capture - all bugs solved! (hope so...)

[33mcommit 1269d923f89dceafb075709440b0a2f9219acd1a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 9 11:45:38 2015 +0100

    small changes in threading. Works well for all 3 cams connected. When 1 cam disconnected -> error

[33mcommit fdc83c4f7b7faaa201d8f539a585c9ee3434496a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 8 18:25:24 2015 +0100

    Capture probleme teilw gelöst - prosilica in eigenem thread. CouldNotBeFilled kommt immer noch vor - Netzwerkproblem?

[33mcommit 85dae39879e7c92bc0368cb776a42945eaba3335[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Jan 8 15:37:45 2015 +0100

    Solved Prosilica startup Bug with workaround (now always correct image after power on)

[33mcommit 59203226a287a0482f6644b0f39dfae42a127afd[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 7 18:12:08 2015 +0100

    trying to make thread terminate... didnt work :(

[33mcommit 0b41a7dbedf0d0ed66ed3784a1f1ccdfd3c25496[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Jan 7 16:44:32 2015 +0100

    RGB_NIR_Depth_Caputre memory leak fixed with QSharedPointer

[33mcommit efe83ef0afd62fefdb332a4269660e4388f52dc4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 6 18:04:37 2015 +0100

    Kinect access implemented in ...CamCapture - no Errors

[33mcommit c8b378ce79bf7e8daa726fbc871dc9bf064e3880[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 6 14:13:10 2015 +0100

    changes in gitignore

[33mcommit 5ea81551cd7390acea164c6a57e1893f12b95f6d[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Jan 6 14:10:15 2015 +0100

    Kinect in ..Capture eingebunden, aber noch fehlerhaft.

[33mcommit 696f2056bffc962f399c8f1a5d5e71a49ad0b207[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Sun Jan 4 13:38:41 2015 +0100

    RGB-to-NIR image fitting parameters can be saved and loaded. RGB&NIR-Stereo-Undistortion implemented

[33mcommit c4584d0437296a591d20833df8bf8379263c26b4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Sat Jan 3 14:29:10 2015 +0100

    rgb to nir fitting (resize & crop) implemented. TODO: try disparity stereo with this.

[33mcommit ef3f3e7d3f9903faed7b78c5a720f4e0bdb1a7ca[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Jan 2 19:14:12 2015 +0100

    image standardization for zero mean and unit variance implemented in helper.h

[33mcommit d16dda27b160d5187fd340aeeb5eb1c182f1d5f3[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Dec 30 14:54:25 2014 +0100

    disparity map improved by using SEED superpixels - OK

[33mcommit fc892b2d29bcbbd828cdb11316bdd024f13590fc[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 29 19:02:48 2014 +0100

    improve disparity with superpixels implemented - still buggy

[33mcommit 291dc14ed1c2f1eb7b1d2302897ae5c710cc68fd[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 29 13:52:43 2014 +0100

    StereoSGBM implemented plus smaller changes

[33mcommit 8b4971d31686106b89047f1e48c2a590a3801661[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Dec 18 16:40:01 2014 +0100

    minor changes

[33mcommit 54aa653ef73ff403964f67e8eed97ce199186c14[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Dec 18 16:21:30 2014 +0100

    stupid workaround to record images... still getting corrupt frames - frame could not be filled to the end

[33mcommit d590725ebe36d313c12223c6792f53e7193dfd4c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Dec 18 10:38:43 2014 +0100

    solved acquisition problem 'framec could not be filled (...)' by simply ignoring it ;)

[33mcommit b1fc23a49fbbc67d013efbefcd6c1772a999ec64[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 17 17:27:35 2014 +0100

    fixed pattern noise and white balance calib imlemented

[33mcommit f26a1531aa0ae571cb379d0e818109a5bcd2e2db[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 17 12:29:19 2014 +0100

    images shown in GUI as QImages

[33mcommit 858cf550c4bc544671b2f6045c22d86248b95bfb[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 17 10:02:53 2014 +0100

    image acquisition start & stop in thread working correctly

[33mcommit 2a42081fdb8b64b8366bc3bdd9c27a6068630110[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 15 18:03:22 2014 +0100

    Image acq. in thread ausgelagert - OK. Thread stoppen funkt noch nicht. Save Images läuft.

[33mcommit 8a9a638c1645000f17ab75d009b6cbbe7cab84e7[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 15 15:18:20 2014 +0100

    goldeye getriggered von flashlight -> works!

[33mcommit acd22ab3dfbd87f2481fee7305f2d55af46a8146[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Dec 12 17:23:15 2014 +0100

    flashlight fuer goldeye funktioniert und triggered. irgendwie kommt die reihenfolge der wavebands noch durcheinander...

[33mcommit 6df21de016d32358e0be935eeed443c8c0a53564[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Dec 12 16:07:57 2014 +0100

    Image Caputure für Prosilica funktioniert

[33mcommit ca175478239b8ee157a2665ba887993ad9858ae4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Dec 12 10:05:24 2014 +0100

    Homogeneity-Matrix-Berechnung fertig (jetzt funktioniert auch das Laden)

[33mcommit 556c501658a41bc1aa7d8ca7cfb8f66bdf93e9f8[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Dec 11 16:23:03 2014 +0100

    homogeneity calibration for goldeye implemented. loading calib matrix still buggy

[33mcommit 6e8bd8cf0b1ac0f18a1e4e0b55f2f3b7a59eb9e3[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 10 16:36:31 2014 +0100

    weiter an RGB_NIR_capture rumgeschraubt... funktioniert noch nicht :(

[33mcommit 1e4528862b9390e869408ea9ff0aca2c4ea4b570[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 10 15:31:29 2014 +0100

    Projekt QuickWebCamCapture hinzugefuegt.

[33mcommit a88f1c1bc8d893f8ac0c6d064b472cd03290c97e[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 10 14:29:31 2014 +0100

    some refactoring on class CameraCalibration

[33mcommit 8b37dcc8b98843155d5599610868d2954349fb59[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 10 13:44:43 2014 +0100

    StereoBM parameter saving/loading + some minor changes

[33mcommit 078022ed78184a5ad3d3b25ba127383ffb949f03[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Dec 9 16:34:27 2014 +0100

    Stereo block matching parameters via sliders

[33mcommit 60d3bae13df3da6ebbd42c8b12df8da768648d25[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 8 17:47:11 2014 +0100

    small changes

[33mcommit 1ca7bae2c62796578eca9db6c61bcda6e05e4bd9[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 8 17:35:39 2014 +0100

    Quick stereo web cam capture implemented. Trying to capture goldeye and prosilica (yet unsuccessful)

[33mcommit b85203eda179067b7571774a7faa6c0d3e371624[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Dec 5 19:34:01 2014 +0100

    Image alignment via features is working + other improvements

[33mcommit e224c82e83ee2895334f616b169ec787930dc800[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Dec 5 12:22:25 2014 +0100

    stereo remapping with dispartity image generation implemented

[33mcommit 04935d54d53f969495f759acb56d9ec15eb75ef5[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Dec 4 19:06:50 2014 +0100

    stereo calibration and remapping is working

[33mcommit b770941279d9bc09554fbf40f5c782cb999a2865[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 3 18:14:23 2014 +0100

    some improvements - goldeye calib/undist

[33mcommit 81ee35cd0f7d585cc89c9324fdb0efdb74c2ec8b[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 3 16:56:08 2014 +0100

    goldeye multichannel calibration and undistort is working

[33mcommit 0814669fdb2cbd232f1c26694f0536e9eba92bff[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Dec 3 11:09:10 2014 +0100

    found corners are displayed in image now

[33mcommit 8af8c0b8340e64f5618ba7607b5709fc082875b4[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Tue Dec 2 18:27:40 2014 +0100

    camera calibration for single cam is working!

[33mcommit 534b706e885f1497a4ec972d94943f84fa654289[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Dec 1 17:41:37 2014 +0100

    Prosilica-Klasse weitergeschrieben. Laeuft aber noch nicht.

[33mcommit d2a499420fa37ee80129b96fbd88bd4cac4acc8e[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Fri Nov 28 16:50:59 2014 +0100

    Bild von Goldeye - OK. Arbeite an Prosilica-Klasse.

[33mcommit 4ca62a9c3960aabc70dd06ffbb06b89b6487e02f[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Nov 27 17:48:04 2014 +0100

    Vimba image acquisition funktioniert (fast)

[33mcommit 4de37a8154ecce4ceea92285e30864cc0bd1e6ae[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 26 16:46:13 2014 +0100

    Versucht Cam-Bild von Prosilica zu bekommen, noch Code-Salat

[33mcommit 4619b747e5aa8aff85e2cb2b53002920fb349aa5[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 26 11:43:11 2014 +0100

    started project for camera capture

[33mcommit b3e85bf76ca443c7cb48399c0d049126aaa33e94[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Mon Nov 17 11:17:20 2014 +0100

    sift ausprobiert

[33mcommit a348c5261f955a9e5da4d9fc83765cfcb9a0035c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Nov 13 14:04:42 2014 +0100

    qt project file added

[33mcommit abc6ebf948ba53a37732e366e5e7566373152744[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Nov 13 14:01:45 2014 +0100

    SURF implementiert

[33mcommit 652a24fd256c212f0bc7e50693b85ff946becc8a[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Thu Nov 13 11:20:09 2014 +0100

    SURF ausprobiert aber features2d macht probleme (SURF nicht gefunden)

[33mcommit 14400019914ef26c768254220671a01ae3fbbd72[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 12 14:58:09 2014 +0100

    Test to see if commit and push/pull really work

[33mcommit 250f6800f1e884f5644e99ced63a8dd381f9fa13[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 12 09:43:00 2014 +0100

    removed test files

[33mcommit 34aa7a8f565f3c0b576d159fddd97426f0150b03[m
Merge: 46d89ce 2005773
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 12 09:35:45 2014 +0100

    Merge branch 'master' of https://github.com/yudanaz/masterthesis
    
     blablaxxxx´´

[33mcommit 46d89cea7d2f9bea9e649a2e1bca201fd37c8a1c[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 12 09:34:22 2014 +0100

    Setting up the REP

[33mcommit 200577349e64e1f5d7178c99895b484bc5ccf708[m
Author: yudanaz <yudanaz@gmail.com>
Date:   Wed Nov 12 09:26:05 2014 +0100

    jetzt die luna zu der pixie

[33mcommit 99b8982674ba8c2930883836459b99e765dc9c10[m
Author: Maurice <yudanaz@gmail.com>
Date:   Wed Nov 12 09:05:34 2014 +0100

    erster commit
