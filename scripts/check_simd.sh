#!/bin/bash

FILE=$1
if [ -z "$FILE" ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

TEMPFILE=$(mktemp)

# Under Linux, you could also decompile your binary:

objdump -d $FILE > $TEMPFILE

# Then find all SSE instructions:

awk '/[ \t](addps|addss|andnps|andps|cmpps|cmpss|comiss|cvtpi2ps|cvtps2pi|cvtsi2ss|cvtss2s|cvttps2pi|cvttss2si|divps|divss|ldmxcsr|maxps|maxss|minps|minss|movaps|movhlps|movhps|movlhps|movlps|movmskps|movntps|movss|movups|mulps|mulss|orps|rcpps|rcpss|rsqrtps|rsqrtss|shufps|sqrtps|sqrtss|stmxcsr|subps|subss|ucomiss|unpckhps|unpcklps|xorps|pavgb|pavgw|pextrw|pinsrw|pmaxsw|pmaxub|pminsw|pminub|pmovmskb|psadbw|pshufw)[ \t]/' $TEMPFILE

# Find only packed SSE instructions (suggested by @Peter Cordes in comments):

awk '/[ \t](addps|andnps|andps|cmpps|cvtpi2ps|cvtps2pi|cvttps2pi|divps|maxps|minps|movaps|movhlps|movhps|movlhps|movlps|movmskps|movntps|movntq|movups|mulps|orps|pavgb|pavgw|pextrw|pinsrw|pmaxsw|pmaxub|pminsw|pminub|pmovmskb|pmulhuw|psadbw|pshufw|rcpps|rsqrtps|shufps|sqrtps|subps|unpckhps|unpcklps|xorps)[ \t]/' $TEMPFILE

# Find all SSE2 instructions (except MOVSD and CMPSD, which were first introduced in 80386):

awk '/[ \t](addpd|addsd|andnpd|andpd|cmppd|comisd|cvtdq2pd|cvtdq2ps|cvtpd2dq|cvtpd2pi|cvtpd2ps|cvtpi2pd|cvtps2dq|cvtps2pd|cvtsd2si|cvtsd2ss|cvtsi2sd|cvtss2sd|cvttpd2dq|cvttpd2pi|cvtps2dq|cvttsd2si|divpd|divsd|maxpd|maxsd|minpd|minsd|movapd|movhpd|movlpd|movmskpd|movupd|mulpd|mulsd|orpd|shufpd|sqrtpd|sqrtsd|subpd|subsd|ucomisd|unpckhpd|unpcklpd|xorpd|movdq2q|movdqa|movdqu|movq2dq|paddq|pmuludq|pshufhw|pshuflw|pshufd|pslldq|psrldq|punpckhqdq|punpcklqdq)[ \t]/' $TEMPFILE

# Find only packed SSE2 instructions:

awk '/[ \t](addpd|andnpd|andpd|cmppd|cvtdq2pd|cvtdq2ps|cvtpd2dq|cvtpd2pi|cvtpd2ps|cvtpi2pd|cvtps2dq|cvtps2pd|cvttpd2dq|cvttpd2pi|cvttps2dq|divpd|maxpd|minpd|movapd|movapd|movhpd|movhpd|movlpd|movlpd|movmskpd|movntdq|movntpd|movupd|movupd|mulpd|orpd|pshufd|pshufhw|pshuflw|pslldq|psrldq|punpckhqdq|shufpd|sqrtpd|subpd|unpckhpd|unpcklpd|xorpd)[ \t]/' $TEMPFILE

#Find all SSE3 instructions:

awk '/[ \t](addsubpd|addsubps|haddpd|haddps|hsubpd|hsubps|movddup|movshdup|movsldup|lddqu|fisttp)[ \t]/' $TEMPFILE

#Find all SSSE3 instructions:

awk '/[ \t](psignw|psignd|psignb|pshufb|pmulhrsw|pmaddubsw|phsubw|phsubsw|phsubd|phaddw|phaddsw|phaddd|palignr|pabsw|pabsd|pabsb)[ \t]/' $TEMPFILE

#Find all SSE4 instructions:

awk '/[ \t](mpsadbw|phminposuw|pmulld|pmuldq|dpps|dppd|blendps|blendpd|blendvps|blendvpd|pblendvb|pblenddw|pminsb|pmaxsb|pminuw|pmaxuw|pminud|pmaxud|pminsd|pmaxsd|roundps|roundss|roundpd|roundsd|insertps|pinsrb|pinsrd|pinsrq|extractps|pextrb|pextrd|pextrw|pextrq|pmovsxbw|pmovzxbw|pmovsxbd|pmovzxbd|pmovsxbq|pmovzxbq|pmovsxwd|pmovzxwd|pmovsxwq|pmovzxwq|pmovsxdq|pmovzxdq|ptest|pcmpeqq|pcmpgtq|packusdw|pcmpestri|pcmpestrm|pcmpistri|pcmpistrm|crc32|popcnt|movntdqa|extrq|insertq|movntsd|movntss|lzcnt)[ \t]/' $TEMPFILE

#Find most common AVX instructions (including scalar, including AVX2, AVX-512 family and some FMA like vfmadd132pd):

awk '/[ \t](vmovapd|vmulpd|vaddpd|vsubpd|vfmadd213pd|vfmadd231pd|vfmadd132pd|vmulsd|vaddsd|vmosd|vsubsd|vbroadcastss|vbroadcastsd|vblendpd|vshufpd|vroundpd|vroundsd|vxorpd|vfnmadd231pd|vfnmadd213pd|vfnmadd132pd|vandpd|vmaxpd|vmovmskpd|vcmppd|vpaddd|vbroadcastf128|vinsertf128|vextractf128|vfmsub231pd|vfmsub132pd|vfmsub213pd|vmaskmovps|vmaskmovpd|vpermilps|vpermilpd|vperm2f128|vzeroall|vzeroupper|vpbroadcastb|vpbroadcastw|vpbroadcastd|vpbroadcastq|vbroadcasti128|vinserti128|vextracti128|vpminud|vpmuludq|vgatherdpd|vgatherqpd|vgatherdps|vgatherqps|vpgatherdd|vpgatherdq|vpgatherqd|vpgatherqq|vpmaskmovd|vpmaskmovq|vpermps|vpermd|vpermpd|vpermq|vperm2i128|vpblendd|vpsllvd|vpsllvq|vpsrlvd|vpsrlvq|vpsravd|vblendmpd|vblendmps|vpblendmd|vpblendmq|vpblendmb|vpblendmw|vpcmpd|vpcmpud|vpcmpq|vpcmpuq|vpcmpb|vpcmpub|vpcmpw|vpcmpuw|vptestmd|vptestmq|vptestnmd|vptestnmq|vptestmb|vptestmw|vptestnmb|vptestnmw|vcompresspd|vcompressps|vpcompressd|vpcompressq|vexpandpd|vexpandps|vpexpandd|vpexpandq|vpermb|vpermw|vpermt2b|vpermt2w|vpermi2pd|vpermi2ps|vpermi2d|vpermi2q|vpermi2b|vpermi2w|vpermt2ps|vpermt2pd|vpermt2d|vpermt2q|vshuff32x4|vshuff64x2|vshuffi32x4|vshuffi64x2|vpmultishiftqb|vpternlogd|vpternlogq|vpmovqd|vpmovsqd|vpmovusqd|vpmovqw|vpmovsqw|vpmovusqw|vpmovqb|vpmovsqb|vpmovusqb|vpmovdw|vpmovsdw|vpmovusdw|vpmovdb|vpmovsdb|vpmovusdb|vpmovwb|vpmovswb|vpmovuswb|vcvtps2udq|vcvtpd2udq|vcvttps2udq|vcvttpd2udq|vcvtss2usi|vcvtsd2usi|vcvttss2usi|vcvttsd2usi|vcvtps2qq|vcvtpd2qq|vcvtps2uqq|vcvtpd2uqq|vcvttps2qq|vcvttpd2qq|vcvttps2uqq|vcvttpd2uqq|vcvtudq2ps|vcvtudq2pd|vcvtusi2ps|vcvtusi2pd|vcvtusi2sd|vcvtusi2ss|vcvtuqq2ps|vcvtuqq2pd|vcvtqq2pd|vcvtqq2ps|vgetexppd|vgetexpps|vgetexpsd|vgetexpss|vgetmantpd|vgetmantps|vgetmantsd|vgetmantss|vfixupimmpd|vfixupimmps|vfixupimmsd|vfixupimmss|vrcp14pd|vrcp14ps|vrcp14sd|vrcp14ss|vrndscaleps|vrndscalepd|vrndscaless|vrndscalesd|vrsqrt14pd|vrsqrt14ps|vrsqrt14sd|vrsqrt14ss|vscalefps|vscalefpd|vscalefss|vscalefsd|valignd|valignq|vdbpsadbw|vpabsq|vpmaxsq|vpmaxuq|vpminsq|vpminuq|vprold|vprolvd|vprolq|vprolvq|vprord|vprorvd|vprorq|vprorvq|vpscatterdd|vpscatterdq|vpscatterqd|vpscatterqq|vscatterdps|vscatterdpd|vscatterqps|vscatterqpd|vpconflictd|vpconflictq|vplzcntd|vplzcntq|vpbroadcastmb2q|vpbroadcastmw2d|vexp2pd|vexp2ps|vrcp28pd|vrcp28ps|vrcp28sd|vrcp28ss|vrsqrt28pd|vrsqrt28ps|vrsqrt28sd|vrsqrt28ss|vgatherpf0dps|vgatherpf0qps|vgatherpf0dpd|vgatherpf0qpd|vgatherpf1dps|vgatherpf1qps|vgatherpf1dpd|vgatherpf1qpd|vscatterpf0dps|vscatterpf0qps|vscatterpf0dpd|vscatterpf0qpd|vscatterpf1dps|vscatterpf1qps|vscatterpf1dpd|vscatterpf1qpd|vfpclassps|vfpclasspd|vfpclassss|vfpclasssd|vrangeps|vrangepd|vrangess|vrangesd|vreduceps|vreducepd|vreducess|vreducesd|vpmovm2d|vpmovm2q|vpmovm2b|vpmovm2w|vpmovd2m|vpmovq2m|vpmovb2m|vpmovw2m|vpmullq|vpmadd52luq|vpmadd52huq|v4fmaddps|v4fmaddss|v4fnmaddps|v4fnmaddss|vp4dpwssd|vp4dpwssds|vpdpbusd|vpdpbusds|vpdpwssd|vpdpwssds|vpcompressb|vpcompressw|vpexpandb|vpexpandw|vpshld|vpshldv|vpshrd|vpshrdv|vpopcntd|vpopcntq|vpopcntb|vpopcntw|vpshufbitqmb|gf2p8affineinvqb|gf2p8affineqb|gf2p8mulb|vpclmulqdq|vaesdec|vaesdeclast|vaesenc|vaesenclast)[ \t]/' $TEMPFILE
 
# NOTE: tested with gawk and nawk.
