# ClonalFrame

![GitHub](https://img.shields.io/github/license/xavierdidelot/ClonalFrame)

<h2>Important notice</h2>
<p>This webpage is about the original ClonalFrame released in 2007 and designed mostly to work on MLST data or small numbers of genomes. In 2015 a new separate software was released called ClonalFrameML which is designed to run on large whole genomes datasets. For more information about ClonalFrameML, see the separate <a href="https://github.com/xavierdidelot/ClonalFrameML">ClonalFrameML website</a>.</p>

<h2>Introduction</h2>
<p>This is the homepage of ClonalFrame, a computer package for the inference of bacterial microevolution using multilocus sequence data.</p>
<p>In a nutshell, ClonalFrame identifies the clonal relationships between the members of a sample, while also estimating the chromosomal position of homologous recombination events that have disrupted the clonal inheritance.</p>
<p>ClonalFrame can be applied to any kind of sequence data, from a single fragment of DNA to whole genomes. It is well suited for the analysis of <a href="http://pubmlst.org/">MLST data</a>, where 7 gene fragments have been sequenced, but becomes progressively more powerful as the sequenced regions increase in length and number up to <a href="http://www.genomesonline.org/">whole genomes</a>. However, it requires the sequences to be aligned. If you have genomic data that is not aligned, we recommend using <a href="http://gel.ahabs.wisc.edu/mauve/">Mauve</a> which produces alignment of whole bacterial genomes in exactly the format required for analysis with ClonalFrame.</p>
<p>The methods used in ClonalFrame are presented in the paper “Inference of bacterial microevolution using multilocus sequence data” by <a href="http://www.genetics.org/cgi/content/abstract/175/3/1251">Didelot and Falush (2007)</a>, which is the appropriate citation for this program.</p>

<h2>Download and Installation</h2>
You can obtain the source code of ClonalFrame by clicking on the "Code" green button at the top of this page and then "Download zip", or via git using the command:

```
git clone https://github.com/xavierdidelot/ClonalFrame
```

You can then compile ClonalFrame using:
```
cd ClonalFrame/src;g++ -lgsl -lgslcblas -lm *.cpp -o ClonalFrame
```

Compilation requires a C++ compiler, such as [GCC](https://gcc.gnu.org/), to be installed. 


<h2>Getting help</h2>
<p>Click here to download the <a href="https://github.com/xavierdidelot/ClonalFrame/raw/main/doc/clonalframe-userguide.pdf">ClonalFrame user guide in PDF format</a>. 
<p>The paper describing the methods is also available: <a href="http://www.genetics.org/cgi/content/abstract/175/3/1251">Didelot and Falush (2007)</a></p>
<p>If you have a question or a problem that is not discussed in the user guide, please <a href="http://xavierdidelot.github.io/contact.html">get in touch</a>.</p>



<h2>xmfa2struct</h2>
<p>xmfa2struct is a program written in C which converts files in eXtended Multi-Fasta format (XMFA, the input format of ClonalFrame) into the input file format of <a href="https://web.stanford.edu/group/pritchardlab/structure.html">Structure</a>. You can find it in the xmfa2struct directory.</p>
