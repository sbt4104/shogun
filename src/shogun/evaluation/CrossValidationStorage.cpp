/*
* BSD 3-Clause License
*
* Copyright (c) 2017, Shogun-Toolbox e.V. <shogun-team@shogun-toolbox.org>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Written (W) 2017 Giovanni De Toni
*
*/

#include "CrossValidationStorage.h"
#include <shogun/labels/Labels.h>
#include <shogun/machine/Machine.h>

#include <shogun/base/range.h>

using namespace shogun;

CrossValidationFoldStorage::CrossValidationFoldStorage() : CEvaluationResult()
{
	m_current_run_index = 0;
	m_current_fold_index = 0;
	m_trained_machine = NULL;
	m_test_result = NULL;
	m_test_true_result = NULL;

	SG_ADD(
	    &m_current_run_index, "run_index",
	    "The current run index of this fold", ParameterProperties::HYPER);
	SG_ADD(
	    &m_current_fold_index, "fold_index", "The current fold index",
	    ParameterProperties::HYPER);
	SG_ADD(
	    &m_trained_machine, "trained_machine",
	    "The machine trained by this fold", ParameterProperties::HYPER);
	SG_ADD(
	    &m_test_result, "predicted_labels",
	    "The test result of this fold", ParameterProperties::HYPER);
	SG_ADD(
	    &m_test_true_result, "ground_truth_labels",
	    "The true test result for this fold", ParameterProperties::HYPER);
	SG_ADD(&m_train_indices, "train_indices",
		   "Indices used for training", ParameterProperties::HYPER);
	SG_ADD(&m_test_indices, "test_indices",
		   "Indices used for testing", ParameterProperties::HYPER);
	SG_ADD(&m_evaluation_result, "evaluation_result",
		   "Result of the evaluation", ParameterProperties::HYPER);

}

CrossValidationFoldStorage::~CrossValidationFoldStorage()
{
	SG_UNREF(m_test_result);
	SG_UNREF(m_test_true_result);
	SG_UNREF(m_trained_machine);
}

void CrossValidationFoldStorage::post_update_results()
{
}

bool CrossValidationFoldStorage::
operator==(const CrossValidationFoldStorage& rhs) const
{
	return m_current_run_index == rhs.m_current_run_index &&
	       m_current_fold_index == rhs.m_current_fold_index &&
	       // m_train_indices.equals(rhs.m_train_indices) &&
	       // m_test_indices.equals(rhs.m_test_indices) &&
	       m_trained_machine->equals(rhs.m_trained_machine) &&
	       m_test_result->equals(rhs.m_test_result) &&
	       m_test_true_result->equals(rhs.m_test_true_result) &&
	       m_evaluation_result == rhs.m_evaluation_result;
}

void CrossValidationFoldStorage::print_result() {

}

/** CrossValidationStorage **/

CrossValidationStorage::CrossValidationStorage() : CEvaluationResult()
{
	m_num_runs = 0;
	m_num_folds = 0;
	m_original_labels = NULL;

	SG_ADD(
	    &m_num_runs, "num_runs", "The total number of cross-validation runs",
	    ParameterProperties::HYPER);
	SG_ADD(
	    &m_num_folds, "num_folds",
	    "The total number of cross-validation folds", ParameterProperties::HYPER);
	SG_ADD(
		&m_original_labels, "labels",
	    "The labels used for this cross-validation", ParameterProperties::HYPER);
	this->watch_param("folds", &m_folds_results, AnyParameterProperties("Fold results"));
}

CrossValidationStorage::~CrossValidationStorage()
{
	SG_UNREF(m_original_labels);
	for (auto i : m_folds_results)
		SG_UNREF(i)
}

void CrossValidationStorage::post_init()
{
}

void CrossValidationStorage::append_fold_result(
    CrossValidationFoldStorage* result)
{
	SG_REF(result);
	m_folds_results.push_back(result);
}

bool CrossValidationStorage::operator==(const CrossValidationStorage& rhs) const
{
	auto member_vars = m_num_runs == rhs.m_num_runs &&
	                   m_num_folds == rhs.m_num_folds &&
	                   m_original_labels->equals(rhs.m_original_labels);

	if (!member_vars)
		return member_vars;

	if (rhs.m_folds_results.size() != m_folds_results.size())
		return false;
	for (auto i : range(m_folds_results.size()))
	{
		if (!(m_folds_results[i] == rhs.m_folds_results[i]))
			return false;
	}
	return member_vars;
}

void CrossValidationStorage::print_result() {

}
