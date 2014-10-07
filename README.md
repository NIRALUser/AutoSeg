#AutoSeg

##What is it?

AutoSeg is a novel C++ based application developped at UNC-Chapel Hill that performs automatic brain tissue classification and structural segmentation.

AutoSeg is designed for use with human and non-human primate pediatric, adolescent and adult data.

AutoSeg uses a BatchMake pipeline script that includes the main steps of the framework entailing N4 bias field correction, rigid registration to a common coordinate image, tissue segmentation, skull-stripping, intensity rescaling, atlas-based registration, subcortical segmentation and lobar parcellation, regional cortical thickness and intensity statistics. AutoSeg allows efficient batch processing and grid computing to process large datasets and provides quality control visualizations via Slicer3 MRML scenes.

##License

See License.txt

##More information

https://www.nitrc.org/projects/autoseg/
