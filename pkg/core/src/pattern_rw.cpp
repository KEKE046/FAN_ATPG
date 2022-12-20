// **************************************************************************
// File       [ pattern_rw.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/10/10 created ]
// **************************************************************************

#include "pattern_rw.h"
#include <fstream>
#include <map>
#include <algorithm>

using namespace IntfNs;
using namespace CoreNs;

// PatternReader
// this method map the PI order to the circuit order
void PatternReader::setPiOrder(const PatNames *const pis)
{
	if (!success_)
	{
		return;
	}

	if (!cir_ || !cir_->nl_)
	{
		success_ = false;
		return;
	}

	pcoll_->numPI_ = 0;
	PatNames *pi = pis->head;
	while (pi)
	{
		++pcoll_->numPI_;
		pi = pi->next;
	}
	// delete [] pcoll_->piOrder_;
	// pcoll_->piOrder_ = new int[pcoll_->numPI_];
	pcoll_->pPIorder_.resize(pcoll_->numPI_);
	pi = pis->head;
	int i = 0;
	while (pi)
	{
		Port *p = cir_->nl_->getTop()->getPort(pi->name);
		if (!p)
		{
			fprintf(stderr, "**ERROR PatternReader::setPiOrder(): port ");
			fprintf(stderr, "`%s' not found\n", pi->name);
			success_ = false;
			// delete[] pcoll_->piOrder_;
			// pcoll_->piOrder_ = NULL;
			pcoll_->pPIorder_.clear();
			return;
		}
		pcoll_->pPIorder_[i] = cir_->portToGate_[p->id_];
		++i;
		pi = pi->next;
	}
}

void PatternReader::setPpiOrder(const PatNames *const ppis)
{
	if (!success_)
	{
		return;
	}

	if (!cir_ || !cir_->nl_)
	{
		success_ = false;
		return;
	}

	pcoll_->numPPI_ = 0;
	PatNames *ppi = ppis->head;
	while (ppi)
	{
		++pcoll_->numPPI_;
		ppi = ppi->next;
	}
	// delete[] pcoll_->ppiOrder_;
	// pcoll_->ppiOrder_ = new int[pcoll_->numPPI_];
	pcoll_->pPPIorder_.resize(pcoll_->numPPI_);
	ppi = ppis->head;
	int i = 0;
	while (ppi)
	{
		Cell *c = cir_->nl_->getTop()->getCell(ppi->name);
		if (!c)
		{
			fprintf(stderr, "**ERROR PatternReader::setPpiOrder(): gate ");
			fprintf(stderr, "`%s' not found\n", ppi->name);
			success_ = false;
			// delete[] pcoll_->ppiOrder_;
			// pcoll_->ppiOrder_ = NULL;
			pcoll_->pPPIorder_.clear();
			return;
		}
		pcoll_->pPPIorder_[i] = cir_->cellToGate_[c->id_];
		++i;
		ppi = ppi->next;
	}
}

void PatternReader::setPoOrder(const PatNames *const pos)
{
	if (!success_)
	{
		return;
	}

	if (!cir_ || !cir_->nl_)
	{
		success_ = false;
		return;
	}
	pcoll_->numPO_ = 0;
	PatNames *po = pos->head;
	while (po)
	{
		++pcoll_->numPO_;
		po = po->next;
	}
	// delete[] pcoll_->poOrder_;
	// pcoll_->poOrder_ = new int[pcoll_->numPO_];
	pcoll_->pPOorder_.resize(pcoll_->numPO_);
	po = pos->head;
	int i = 0;
	while (po)
	{
		Port *p = cir_->nl_->getTop()->getPort(po->name);
		if (!p)
		{
			fprintf(stderr, "**ERROR PatternReader::setPoOrder(): port ");
			fprintf(stderr, "`%s' not found\n", po->name);
			success_ = false;
			// delete[] pcoll_->poOrder_;
			// pcoll_->poOrder_ = NULL;
			pcoll_->pPOorder_.clear();
			return;
		}
		pcoll_->pPOorder_[i] = cir_->portToGate_[p->id_];
		++i;
		po = po->next;
	}
}

void PatternReader::setPatternType(const PatType &type)
{
	if (!success_)
	{
		return;
	}

	switch (type)
	{
		case IntfNs::BASIC_SCAN:
			pcoll_->type_ = PatternProcessor::BASIC_SCAN;
			break;
		case IntfNs::LAUNCH_CAPTURE:
			pcoll_->type_ = PatternProcessor::LAUNCH_CAPTURE;
			break;
		case IntfNs::LAUNCH_SHIFT:
			pcoll_->type_ = PatternProcessor::LAUNCH_SHIFT;
			pcoll_->numSI_ = 1;
			break;
	}
}

void PatternReader::setPatternNum(const int &num)
{
	if (!success_)
	{
		return;
	}
	pcoll_->patternVector_.resize(num);
	for (size_t i = 0; i < pcoll_->patternVector_.size(); ++i)
	{
		pcoll_->patternVector_[i] = Pattern();
	}
	curPat_ = 0;
}

// read in a pattern
void PatternReader::addPattern(const char *const pi1,
															 const char *const pi2,
															 const char *const ppi,
															 const char *const si,
															 const char *const po1,
															 const char *const po2,
															 const char *const ppo)
{
	if (!success_)
	{
		return;
	}
	if (pi1 && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].primaryInputs1st_.resize(pcoll_->numPI_);
		assignValue(pcoll_->patternVector_[curPat_].primaryInputs1st_, pi1, pcoll_->numPI_);
	}
	if (pi2 && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].primaryInputs2nd_.resize(pcoll_->numPI_);
		assignValue(pcoll_->patternVector_[curPat_].primaryInputs2nd_, pi2, pcoll_->numPI_);
	}
	if (ppi && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].pseudoPrimaryInputs_.resize(pcoll_->numPPI_);
		assignValue(pcoll_->patternVector_[curPat_].pseudoPrimaryInputs_, ppi, pcoll_->numPPI_);
	}
	if (si && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].shiftIn_.resize(pcoll_->numSI_);
		assignValue(pcoll_->patternVector_[curPat_].shiftIn_, si, pcoll_->numSI_);
	}
	if (po1 && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].primaryOutputs1st_.resize(pcoll_->numPO_);
		assignValue(pcoll_->patternVector_[curPat_].primaryOutputs1st_, po1, pcoll_->numPO_);
	}
	if (po2 && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].primaryOutputs2nd_.resize(pcoll_->numPO_);
		assignValue(pcoll_->patternVector_[curPat_].primaryOutputs2nd_, po2, pcoll_->numPO_);
	}
	if (ppo && curPat_ < (int)pcoll_->patternVector_.size())
	{
		pcoll_->patternVector_[curPat_].pseudoPrimaryOutputs_.resize(pcoll_->numPPI_);
		assignValue(pcoll_->patternVector_[curPat_].pseudoPrimaryOutputs_, ppo, pcoll_->numPPI_);
	}
	++curPat_;
}

void PatternReader::assignValue(std::vector<Value> &v, const char *const pat,
																const int &size)
{
	for (int i = 0; i < size; ++i)
	{
		switch (pat[i])
		{
			case '0':
				v[i] = L;
				break;
			case '1':
				v[i] = H;
				break;
			default:
				v[i] = X;
				break;
		}
		// if (pat[i] == '0')
		// 	v[i] = L;
		// else if (pat[i] == '1')
		// 	v[i] = H;
		// else
		// 	v[i] = X;
	}
}

// PatternWriter

// write to LaDS's own *.pat  pattern format
// support 2 time frames, but no more than 2 time frames
bool PatternWriter::writePat(const char *const fname)
{
	FILE *fout = fopen(fname, "w");
	if (!fout)
	{
		fprintf(stderr, "**ERROR PatternWriter::writePat(): file");
		fprintf(stderr, "`%s' cannot be opened\n", fname);
		return false;
	}

	for (int i = 0; i < cir_->npi_; ++i)
	{
		fprintf(fout, "%s ",
						cir_->nl_->getTop()->getPort(cir_->gates_[i].cellId_)->name_);
	}
	fprintf(fout, " |\n");

	for (int i = cir_->npi_; i < cir_->npi_ + cir_->nppi_; ++i)
	{
		fprintf(fout, "%s ",
						cir_->nl_->getTop()->getCell(cir_->gates_[i].cellId_)->name_);
	}
	fprintf(fout, " |\n");

	int start = cir_->npi_ + cir_->nppi_ + cir_->ncomb_;
	for (int i = start; i < start + cir_->npo_; ++i)
	{
		fprintf(fout, "%s ",
						cir_->nl_->getTop()->getPort(cir_->gates_[i].cellId_)->name_);
	}
	fprintf(fout, "\n");

	switch (pcoll_->type_)
	{
		case PatternProcessor::BASIC_SCAN:
			fprintf(fout, "BASIC_SCAN\n");
			break;
		case PatternProcessor::LAUNCH_CAPTURE:
			fprintf(fout, "LAUNCH_ON_CAPTURE\n");
			break;
		case PatternProcessor::LAUNCH_SHIFT:
			fprintf(fout, "LAUNCH_ON_SHIFT\n");
			break;
	}

	fprintf(fout, "_num_of_pattern_%d\n", (int)pcoll_->patternVector_.size());

	for (int i = 0; i < (int)pcoll_->patternVector_.size(); ++i)
	{
		fprintf(fout, "_pattern_%d ", i + 1);
		if (!pcoll_->patternVector_[i].primaryInputs1st_.empty())
		{
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].primaryInputs2nd_.empty())
		{
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].pseudoPrimaryInputs_.empty())
		{
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].shiftIn_.empty())
		{
			for (int j = 0; j < pcoll_->numSI_; ++j)
			{
				if (pcoll_->patternVector_[i].shiftIn_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].shiftIn_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].primaryOutputs1st_.empty())
		{
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].primaryOutputs2nd_.empty())
		{
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");
		if (!pcoll_->patternVector_[i].pseudoPrimaryInputs_.empty())
		{
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
	fclose(fout);

	return true;
} //}}}

//{{{ bool PatternWriter::writeLht(const char * const)

// write to Ling Hsio-Ting's pattern format
// This format is OBSOLETE, no longer supported
//
bool PatternWriter::writeLht(const char *const fname)
{
	FILE *fout = fopen(fname, "w");
	if (!fout)
	{
		fprintf(stderr, "**ERROR PatternWriter::writePat(): file");
		fprintf(stderr, "`%s' cannot be opened\n", fname);
		return false;
	}

	for (size_t i = 0; i < pcoll_->patternVector_.size(); ++i)
	{
		fprintf(fout, "%d: ", (int)i + 1);
		if (!pcoll_->patternVector_[i].primaryInputs1st_.empty())
		{
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		if (!pcoll_->patternVector_[i].primaryInputs2nd_.empty())
		{
			fprintf(fout, "->");
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, "_");
		if (!pcoll_->patternVector_[i].pseudoPrimaryInputs_.empty())
		{
			fprintf(fout, "->");
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		if (!pcoll_->patternVector_[i].shiftIn_.empty())
		{
			fprintf(fout, "@");
			for (int j = 0; j < pcoll_->numSI_; ++j)
			{
				if (pcoll_->patternVector_[i].shiftIn_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].shiftIn_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, " | ");

		if (!pcoll_->patternVector_[i].primaryOutputs1st_.empty() && pcoll_->type_ == PatternProcessor::BASIC_SCAN)
		{
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}

		if (!pcoll_->patternVector_[i].primaryOutputs2nd_.empty() && (pcoll_->type_ == PatternProcessor::LAUNCH_CAPTURE || pcoll_->type_ == PatternProcessor::LAUNCH_SHIFT))
		{
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, "_");
		if (!pcoll_->patternVector_[i].pseudoPrimaryOutputs_.empty())
		{
			fprintf(fout, "->");
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
	fclose(fout);

	return true;
} //}}}

// write to Mentor ASCii
// E.5 problem
// must test with mentor fastscan
bool PatternWriter::writeAscii(const char *const fname)
{
	FILE *fout = fopen(fname, "w");
	int first_flag = 1;
	int seqCircuitCheck = 0;
	if (!fout)
	{
		fprintf(stderr, "**ERROR PatternWriter::writePat(): file");
		fprintf(stderr, "`%s' cannot be opened\n", fname);
		return false;
	}
	fprintf(fout, "ASCII_PATTERN_FILE_VERSION = 2;\n");
	fprintf(fout, "SETUP =\n");

	// input
	fprintf(fout, "declare input bus \"PI\" = ");
	// fprintf(fout, "\"/CK\", \"/test_si\", \"/test_se\"");
	for (size_t i = 0; i < cir_->nl_->getTop()->getNPort(); ++i)
	{
		Port *p = cir_->nl_->getTop()->getPort(i);
		if (p->type_ != Port::INPUT)
			continue;
		if (first_flag)
		{
			fprintf(fout, "\"/%s\"",
							cir_->nl_->getTop()->getPort(i)->name_);
			first_flag = 0;
		}
		else
		{
			fprintf(fout, ", \"/%s\"",
							cir_->nl_->getTop()->getPort(i)->name_);
		}
		if (!strcmp(p->name_, "CK"))
			seqCircuitCheck = 1;
	}
	fprintf(fout, ";\n");
	first_flag = 1;
	// output
	fprintf(fout, "declare output bus \"PO\" = ");
	for (size_t i = 0; i < cir_->nl_->getTop()->getNPort(); ++i)
	{
		Port *p = cir_->nl_->getTop()->getPort(i);
		if (p->type_ != Port::OUTPUT)
			continue;
		if (first_flag)
		{
			fprintf(fout, "\"/%s\"",
							cir_->nl_->getTop()->getPort(i)->name_);
			first_flag = 0;
		}
		else
		{
			fprintf(fout, ", \"/%s\"",
							cir_->nl_->getTop()->getPort(i)->name_);
		}
	}
	fprintf(fout, ";\n");

	// clock
	if (seqCircuitCheck)
	{
		fprintf(fout, "clock \"/CK\" =\n");
		fprintf(fout, "    off_state = 0;\n");
		fprintf(fout, "    pulse_width = 1;\n");
		fprintf(fout, "end;\n");

		// test setup
		fprintf(fout, "procedure test_setup \"test_setup\" =\n");
		fprintf(fout, "    force \"/CK\" 0 0;\n");
		fprintf(fout, "    force \"/test_si\" 0 0;\n");
		fprintf(fout, "    force \"/test_se\" 0 0;\n");
		fprintf(fout, "end;\n");

		// scan group
		fprintf(fout, "scan_group \"group1\" =\n");
		fprintf(fout, "    scan_chain \"chain1\" =\n");
		fprintf(fout, "    scan_in = \"/test_si\";\n");
		fprintf(fout, "    scan_out = \"/test_so\";\n");
		fprintf(fout, "    length = %d;\n", cir_->nppi_);
		fprintf(fout, "    end;\n");

		fprintf(fout, "    procedure shift \"group1_load_shift\" =\n");
		fprintf(fout, "    force_sci \"chain1\" 0;\n");
		fprintf(fout, "    force \"/CK\" 1 16;\n");
		fprintf(fout, "    force \"/CK\" 0 19;\n");
		fprintf(fout, "    period 32;\n");
		fprintf(fout, "    end;\n");

		fprintf(fout, "    procedure shift \"group1_unload_shift\" =\n");
		fprintf(fout, "    measure_sco \"chain1\" 0;\n");
		fprintf(fout, "    force \"/CK\" 1 16;\n");
		fprintf(fout, "    force \"/CK\" 0 19;\n");
		fprintf(fout, "    period 32;\n");
		fprintf(fout, "    end;\n");

		fprintf(fout, "    procedure load \"group1_load\" =\n");
		fprintf(fout, "    force \"/CK\" 0 0;\n");
		fprintf(fout, "    force \"/test_se\" 1 0;\n");
		fprintf(fout, "    force \"/test_si\" 0 0;\n");
		fprintf(fout, "    apply \"group1_load_shift\" %d 32;\n", cir_->nppi_);
		fprintf(fout, "    end;\n");

		fprintf(fout, "    procedure unload \"group1_unload\" =\n");
		fprintf(fout, "    force \"/CK\" 0 0;\n");
		fprintf(fout, "    force \"/test_se\" 1 0;\n");
		fprintf(fout, "    force \"/test_si\" 0 0;\n");
		fprintf(fout, "    apply \"group1_unload_shift\" %d 32;\n", cir_->nppi_);
		fprintf(fout, "    end;\n");

		fprintf(fout, "end;\n");
	}
	fprintf(fout, "end;\n\n");

	fprintf(fout, "SCAN_TEST =\n");
	// fprintf(fout, "apply \"test_setup\" 1 0;\n");

	for (int i = 0; i < (int)pcoll_->patternVector_.size(); ++i)
	{
		fprintf(fout, "pattern = %d", i);
		if (!pcoll_->patternVector_[i].primaryInputs2nd_.empty())
			fprintf(fout, " clock_sequential;\n");
		else
			fprintf(fout, ";\n");

		if (pcoll_->numPPI_)
		{
			fprintf(fout, "apply  \"group1_load\" 0 =\n");
			fprintf(fout, "chain \"chain1\" = \"");
			for (int j = pcoll_->numPPI_ - 1; j >= 0; --j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\";\n");
			fprintf(fout, "end;\n");
		}
		if (!pcoll_->patternVector_[i].primaryInputs1st_.empty())
		{
			fprintf(fout, "force \"PI\" \"");
			if (seqCircuitCheck)
				fprintf(fout, "000");
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\" 1;\n");
		}
		// fprintf(fout, "pulse \"/CK\" 2;\n");
		if (!pcoll_->patternVector_[i].primaryInputs2nd_.empty())
		{
			if (seqCircuitCheck)
				fprintf(fout, "pulse \"/CK\" 2;\n");
			fprintf(fout, "force \"PI\" \"");
			if (seqCircuitCheck)
				fprintf(fout, "000");
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryInputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\" 3;\n");
		}
		if (!pcoll_->patternVector_[i].primaryOutputs2nd_.empty())
		{
			fprintf(fout, "measure \"PO\" \"");
			if (seqCircuitCheck)
				fprintf(fout, "X");
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\" 4;\n");
		}
		else if (!pcoll_->patternVector_[i].primaryOutputs1st_.empty())
		{
			fprintf(fout, "measure \"PO\" \"");
			if (seqCircuitCheck)
				fprintf(fout, "X");
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\" 4;\n");
		}
		/*if (pcoll_->patternVector_[i].primaryOutputs2nd_) {
				fprintf(fout, "pulse \"/CK\" 5;\n");
				fprintf(fout, "measure \"PO\" \"");
				if (seqCircuitCheck)
						fprintf(fout, "X");
				for (int j = 0; j < pcoll_->numPO_; ++j) {
						if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == L)
								fprintf(fout, "0");
						else if (pcoll_->patternVector_[i].primaryOutputs2nd_[j] == H)
								fprintf(fout, "1");
						else
								fprintf(fout, "X");
				}
				fprintf(fout, "\" 6;\n");
		}*/

		if (pcoll_->numPPI_)
		{
			fprintf(fout, "pulse \"/CK\" 5;\n");
			fprintf(fout, "apply  \"group1_unload\" 6 =\n");
			fprintf(fout, "chain \"chain1\" = \"");
			for (int j = pcoll_->numPPI_ - 1; j >= 0; --j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == L)
					fprintf(fout, "0");
				else if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == H)
					fprintf(fout, "1");
				else
					fprintf(fout, "X");
			}
			fprintf(fout, "\";\n");
			fprintf(fout, "end;\n");
		}
	}
	fprintf(fout, "end;\n");

	// scan cells
	if (seqCircuitCheck)
	{
		fprintf(fout, "SCAN_CELLS =\n");
		fprintf(fout, "scan_group \"group1\" =\n");
		fprintf(fout, "scan_chain \"chain1\" =\n");
		for (int i = cir_->npi_ + cir_->nppi_ - 1; i >= cir_->npi_; --i)
		{
			fprintf(fout,
							"scan_cell = %d MASTER FFFF \"/%s\" \"I1\" \"SI\" \"Q\";\n",
							cir_->npi_ + cir_->nppi_ - 1 - i, cir_->nl_->getTop()->getCell(cir_->gates_[i].cellId_)->name_);
		}

		fprintf(fout, "end;\n");
		fprintf(fout, "end;\n");
		fprintf(fout, "end;\n");
	}
	fprintf(fout, "\n");
	fclose(fout);

	return true;
}

// 2016 summer train
//  write to STIL
//  must test with tetramax
bool PatternWriter::writeSTIL(const char *const fname)
{

	// TODO

	//
	std::vector<std::string> PI_Order;
	std::vector<std::string> SCAN_Order;
	std::vector<std::string> PO_Order;

	std::cout << "==========\n"; //

	PI_Order.push_back("CK");
	PI_Order.push_back("test_si");
	PI_Order.push_back("test_se");
	PO_Order.push_back("test_so");

	for (int i = 0; i < cir_->npi_; ++i)
	{
		PI_Order.push_back(cir_->nl_->getTop()->getPort(cir_->gates_[i].cellId_)->name_);
	}

	for (int i = cir_->npi_; i < cir_->npi_ + cir_->nppi_; ++i)
	{
		SCAN_Order.push_back(cir_->nl_->getTop()->getCell(cir_->gates_[i].cellId_)->name_);
	}

	int start = cir_->npi_ + cir_->nppi_ + cir_->ncomb_;
	for (int i = start; i < start + cir_->npo_; ++i)
	{
		PO_Order.push_back(cir_->nl_->getTop()->getPort(cir_->gates_[i].cellId_)->name_);
	}

	std::cout << "PI_ORDER ";
	for (size_t i = 0; i < PI_Order.size(); ++i)
	{
		std::cout << PI_Order[i] << " ";
	}
	std::cout << "\n";

	std::cout << "SCAN_ORDER ";
	for (size_t i = 0; i < SCAN_Order.size(); ++i)
		std::cout << SCAN_Order[i] << " ";
	std::cout << "\n";

	std::cout << "PO_ORDER ";
	for (size_t i = 0; i < PO_Order.size(); ++i)
		std::cout << PO_Order[i] << " ";
	std::cout << "\n";

	std::string Processor_Mode = "";
	int pattern_size = (int)pcoll_->patternVector_.size();

	switch (pcoll_->type_)
	{
		case PatternProcessor::BASIC_SCAN:
			Processor_Mode = "BASIC_SCAN";
			break;
		case PatternProcessor::LAUNCH_CAPTURE:
			Processor_Mode = "LAUNCH_ON_CAPTURE";
			break;
		case PatternProcessor::LAUNCH_SHIFT:
			Processor_Mode = "LAUNCH_ON_SHIFT";
			break;
	}

	std::cout << Processor_Mode << " " << pattern_size << "\n"; //

	std::vector<std::map<std::string, std::string>> patternList;

	for (int i = 0; i < (int)pcoll_->patternVector_.size(); ++i)
	{
		std::map<std::string, std::string> map_pattern;

		if (!pcoll_->patternVector_[i].primaryInputs1st_.empty())
		{
			map_pattern["pi1"] = "";
			for (int j = 0; j < pcoll_->numPI_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryInputs1st_[j] == L)
					map_pattern["pi1"] += "0";
				else if (pcoll_->patternVector_[i].primaryInputs1st_[j] == H)
					map_pattern["pi1"] += "1";
				else
					map_pattern["pi1"] += "N";
			}
		}

		if (!pcoll_->patternVector_[i].pseudoPrimaryInputs_.empty())
		{
			map_pattern["ppi1"] = "";
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == L)
					map_pattern["ppi1"] += "0";
				else if (pcoll_->patternVector_[i].pseudoPrimaryInputs_[j] == H)
					map_pattern["ppi1"] += "1";
				else
					map_pattern["ppi1"] += "N";
			}
			reverse(map_pattern["ppi1"].begin(), map_pattern["ppi1"].end());
		}
		if (!pcoll_->patternVector_[i].primaryOutputs1st_.empty())
		{
			map_pattern["po1"] = "";
			for (int j = 0; j < pcoll_->numPO_; ++j)
			{
				if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == L)
					map_pattern["po1"] += "L";
				else if (pcoll_->patternVector_[i].primaryOutputs1st_[j] == H)
					map_pattern["po1"] += "H";
				else
					map_pattern["po1"] += "N";
			}
		}

		if (!pcoll_->patternVector_[i].pseudoPrimaryInputs_.empty())
		{
			map_pattern["ppo"] = "";
			for (int j = 0; j < pcoll_->numPPI_; ++j)
			{
				if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == L)
					map_pattern["ppo"] += "L";
				else if (pcoll_->patternVector_[i].pseudoPrimaryOutputs_[j] == H)
					map_pattern["ppo"] += "H";
				else
					map_pattern["ppo"] += "N";
			}
			std::reverse(map_pattern["ppo"].begin(), map_pattern["ppo"].end());
		}
		patternList.push_back(map_pattern);
	}

	for (size_t i = 0; i < patternList.size(); ++i)
	{
		std::cout << "\npattern_" << i + 1 << "\n";
		std::map<std::string, std::string>::iterator iter;
		for (iter = patternList[i].begin(); iter != patternList[i].end(); iter++)
			std::cout << iter->first << " " << iter->second << "\n";
	} //

	std::cout << "==========\n";

	std::ofstream os(fname);
	os << "STIL 1.0;\n\n";

	/////signals/////
	os << "Signals {\n";
	for (size_t i = 0; i < PI_Order.size(); ++i)
	{
		os << "   \"" + PI_Order[i] + "\" " + "In";
		if (PI_Order[i] == "test_si")
		{
			os << " { ScanIn; }\n";
		}
		else
		{
			os << ";\n";
		}
	}
	for (size_t i = 0; i < PO_Order.size(); ++i)
	{
		os << "   \"" + PO_Order[i] + "\" " + "Out";
		if (PO_Order[i] == "test_so")
		{
			os << " { ScanOut; }\n";
		}
		else
		{
			os << ";\n";
		}
	}
	os << "}\n\n";
	/////SignalGroups/////
	os << "SignalGroups {\n";
	std::string _pi_in = "";
	std::string _po_out = "";
	std::string _in_timing = "";
	for (size_t i = 0; i < PI_Order.size(); ++i)
	{
		_pi_in = _pi_in + "\"" + PI_Order[i] + "\"";
		if (i != PI_Order.size() - 1)
		{
			_pi_in += " + ";
		}

		if (PI_Order[i] != "CK")
		{
			_in_timing = _in_timing + "\"" + PI_Order[i] + "\"";
			if (i != PI_Order.size() - 1)
			{
				_in_timing += " + ";
			}
		}
	}
	for (size_t i = 0; i < PO_Order.size(); ++i)
	{
		_po_out = _po_out + "\"" + PO_Order[i] + "\"";
		if (i != PO_Order.size() - 1)
		{
			_po_out += " + ";
		}
	}
	os << "   \"_pi\" = \'" + _pi_in + "\';\n";
	os << "   \"_in\" = \'" + _pi_in + "\';\n";
	os << "   \"_si\" = \'\"test_si\"\' { ScanIn; }\n";
	os << "   \"_po\" = \'" + _po_out + "\';\n";
	os << "   \"_out\" = \'" + _po_out + "\';\n";
	os << "   \"_so\" = \'\"test_so\"\' { ScanOut; }\n";
	os << "   \"_default_In_Timing_\" = \'" + _in_timing + "\';\n";
	os << "   \"_default_Out_Timing_\" = \'" + _po_out + "\';\n";
	os << "}\n\n";
	/////Timing/////
	os << "Timing {\n";
	os << "   WaveformTable \"_default_WFT_\" {\n";
	os << "       Period \'100ns\';\n";
	os << "       Waveforms {\n";
	os << "           \"CK\" { 0 { \'0ns\' D; } }\n";
	os << "           \"CK\" { P { \'0ns\' D; \'50ns\' U; \'75ns\' D; } }\n";
	os << "           \"CK\" { 1 { \'0ns\' U; } }\n";
	os << "           \"CK\" { Z { \'0ns\' Z; } }\n";
	os << "           \"_default_In_Timing_\" { 0 { \'0ns\' D; } }\n";
	os << "           \"_default_In_Timing_\" { 1 { \'0ns\' U; } }\n";
	os << "           \"_default_In_Timing_\" { Z { \'0ns\' Z; } }\n";
	os << "           \"_default_In_Timing_\" { N { \'0ns\' N; } }\n";
	os << "           \"_default_Out_Timing_\" { X { \'0ns\' X; } }\n";
	os << "           \"_default_Out_Timing_\" { H { \'0ns\' X; \' 90 ns\' H; } }\n";
	os << "           \"_default_Out_Timing_\" { T { \'0ns\' X; \' 90 ns\' T; } }\n";
	os << "           \"_default_Out_Timing_\" { L { \'0ns\' X; \' 90 ns\' L; } }\n";
	os << "       }\n";
	os << "   }\n";
	os << "}\n\n";

	/////ScanStructures/////
	os << "ScanStructures {\n";
	os << "   ScanChain \"chain1\" {\n"; //
	os << "       ScanLength " << SCAN_Order.size() << ";\n";
	os << "       ScanIn \"test_si\";\n";
	os << "       ScanOut \"test_so\";\n";
	os << "       ScanInversion 0;\n";
	os << "       ScanCells";
	for (size_t i = 0; i < SCAN_Order.size(); ++i)
	{
		os << " \"TOP." << SCAN_Order[i] << ".SI\"";
	}
	os << ";\n";
	os << "       ScanMasterClock \"CK\" ;\n";
	os << "   }\n";
	os << "}\n\n";

	/////PatternBurst PatternExec/////
	os << "PatternBurst \"_burst_\" {\n";
	os << "   PatList { \"_pattern_\" { } }\n";
	os << "}\n\n";
	os << "PatternExec {\n";
	os << "   PatternBurst \"_burst_\";\n";
	os << "}\n\n";

	/////Procedures/////
	os << "Procedures {\n";
	os << "   \"load_unload\" {\n";
	os << "       W \"_default_WFT_\";\n";
	os << "       C { \"test_si\"=0; \"CK\"=0; \"test_se\"=1; }\n";
	os << "       V { \"_so\"=#; }\n";
	os << "       Shift {\n";
	os << "           W \"_default_WFT_\";\n";
	os << "           V { \"_si\"=#; \"_so\"=#; \"CK\"=P; }\n";
	os << "       }\n";
	os << "   }\n";
	os << "   \"capture_CK\" {\n";
	os << "       W \"_default_WFT_\";\n";
	os << "       F { \"test_se\"=0; }\n";
	os << "       C { \"_po\"=\\r" << PO_Order.size() << " X ; }\n";
	os << "       \"forcePI\": V { \"_pi\"=\\r" << PI_Order.size() << " # ; }\n";
	os << "       \"measurePO\": V { \"_po\"=\\r" << PO_Order.size() << " # ; }\n";
	os << "       C { \"_po\"=\\r" << PO_Order.size() << " X ; }\n";
	os << "       \"pulse\": V { \"CK\"=P; }\n";
	os << "   }\n";
	os << "   \"capture\" {\n";
	os << "       W \"_default_WFT_\";\n";
	os << "       F { \"test_se\"=0; }\n";
	os << "       C { \"_po\"=\\r" << PO_Order.size() << " X ; }\n";
	os << "       \"forcePI\": V { \"_pi\"=\\r" << PI_Order.size() << " # ; }\n";
	os << "       \"measurePO\": V { \"_po\"=\\r" << PO_Order.size() << " # ; }\n";
	os << "   }\n";
	os << "}\n\n";

	/////MacroDefs/////
	os << "MacroDefs {\n";
	os << "   \"test_setup\" {\n";
	os << "       W \"_default_WFT_\";\n";
	os << "       V { \"test_se\"=0; \"CK\"=0; }\n";
	os << "   }\n";
	os << "}\n\n";

	/////Pattern/////
	os << "Pattern \"_pattern_\" {\n";
	os << "   W \"_default_WFT_\";\n";
	os << "   \"precondition all Signals\": C { \"_pi\"=\\r" << PI_Order.size() << " 0 ; \"_po\"=\\r" << PO_Order.size() << " X ; }\n";
	os << "   Macro \"test_setup\";\n";

	for (int i = 0; i < pattern_size; ++i)
	{
		os << "   \"pattern " << i << "\":\n";
		os << "       Call \"load_unload\" {\n";
		if (i > 0)
		{
			os << "           \"test_so\"=" << patternList[i - 1]["ppo"] << ";\n";
		}
		os << "           \"test_si\"=" << patternList[i]["ppi1"] << ";\n";
		os << "       }\n";
		os << "       Call \"capture_CK\" {\n";
		os << "           \"_pi\"=";
		for (size_t j = patternList[i]["pi1"].length(); j < PI_Order.size(); ++j)
		{
			os << "0";
		}
		os << patternList[i]["pi1"] << ";\n";
		os << "           \"_po\"=";
		for (size_t j = patternList[i]["po1"].length(); j < PO_Order.size(); ++j)
		{
			os << "L";
		}
		os << patternList[i]["po1"] << ";\n";
		os << "       }\n";
	}

	os << "   \"end " << pattern_size - 1 << " unload\":\n";
	os << "       Call \"load_unload\" {\n";
	os << "           \"test_so\"=" << patternList[pattern_size - 1]["ppo"] << ";\n";
	os << "       }\n";
	os << "}";

	os.close();

	return true;
}

bool ProcedureWriter::writeProc(const char *const fname)
{
	FILE *fout = fopen(fname, "w");
	if (!fout)
	{
		fprintf(stderr, "**ERROR PatternWriter::writePat(): file");
		fprintf(stderr, "`%s' cannot be opened\n", fname);
		return false;
	}

	fprintf(fout, "set time scale 1.000000 ns ;\n");
	fprintf(fout, "set strobe_window time 3 ;\n\n");
	fprintf(fout, "timeplate _default_WFT_ =\n");
	fprintf(fout, "force_pi 0 ;\n");
	fprintf(fout, "measure_po 29 ;\n");
	fprintf(fout, "pulse CK 16 3;\n");
	fprintf(fout, "period 32 ;\n");
	fprintf(fout, "end;\n\n");
	fprintf(fout, "procedure shift =\n");
	fprintf(fout, "scan_group group1 ;\n");
	fprintf(fout, "timeplate _default_WFT_ ;\n");
	fprintf(fout, "cycle =\n");
	fprintf(fout, "force_sci ;\n");
	fprintf(fout, "pulse CK ;\n");
	fprintf(fout, "measure_sco ;\n");
	fprintf(fout, "end;\n");
	fprintf(fout, "end;\n\n");
	fprintf(fout, "procedure load_unload =\n");
	fprintf(fout, "scan_group group1 ;\n");
	fprintf(fout, "timeplate _default_WFT_ ;\n");
	fprintf(fout, "cycle =\n");
	fprintf(fout, "force CK 0 ;\n");
	fprintf(fout, "force test_se 1 ;\n");
	fprintf(fout, "force test_si 0 ;\n");
	fprintf(fout, "measure_sco ;\n");
	fprintf(fout, "end ;\n");
	fprintf(fout, "apply shift %d;\n", cir_->nppi_);
	fprintf(fout, "end;\n");
	fprintf(fout, "procedure test_setup =\n");
	fprintf(fout, "timeplate _default_WFT_ ;\n");
	fprintf(fout, "cycle =\n");
	fprintf(fout, "force CK 0 ;\n");
	fprintf(fout, "force test_se 0 ;\n");
	fprintf(fout, "end;\n");
	fprintf(fout, "end;\n");
	fclose(fout);
	return true;
}
