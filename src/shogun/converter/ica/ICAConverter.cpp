/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Kevin Hughes, Heiko Strathmann, Bjoern Esser
 */

#include <shogun/converter/ica/ICAConverter.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/mathematics/eigen3.h>
#include <shogun/mathematics/linalg/LinalgNamespace.h>

using namespace shogun;
using namespace Eigen;

CICAConverter::CICAConverter() : CConverter()
{
	init();
}

void CICAConverter::init()
{
	m_mixing_matrix = SGMatrix<float64_t>();
	max_iter = 200;
	tol = 1e-6;

	SG_ADD(&m_mixing_matrix, "mixing_matrix", "the mixing matrix");
	SG_ADD(&max_iter, "max_iter", "maximum number of iterations");
	SG_ADD(&tol, "tol", "the convergence tolerance");
}

CICAConverter::~CICAConverter()
{
}

void CICAConverter::set_mixing_matrix(SGMatrix<float64_t> mixing_matrix)
{
	m_mixing_matrix = mixing_matrix;
}

SGMatrix<float64_t> CICAConverter::get_mixing_matrix() const
{
	return m_mixing_matrix;
}

void CICAConverter::set_max_iter(int iter)
{
	max_iter = iter;
}

int CICAConverter::get_max_iter() const
{
	return max_iter;
}

void CICAConverter::set_tol(float64_t _tol)
{
	tol = _tol;
}

float64_t CICAConverter::get_tol() const
{
	return tol;
}

void CICAConverter::fit(CFeatures* features)
{
	REQUIRE(features, "Features are not provided\n");
	REQUIRE(
	    features->get_feature_class() == C_DENSE,
	    "ICA converters only work with dense features\n");
	REQUIRE(
	    features->get_feature_type() == F_DREAL,
	    "ICA converters only work with real features\n");

	fit_dense(static_cast<CDenseFeatures<float64_t>*>(features));
}

CFeatures* CICAConverter::transform(CFeatures* features, bool inplace)
{
	REQUIRE(m_mixing_matrix.matrix, "ICAConverter has not been fitted.\n");

	auto X = features->as<CDenseFeatures<float64_t>>()->get_feature_matrix();
	if (!inplace)
		X = X.clone();

	Map<MatrixXd> EX(X.matrix, X.num_rows, X.num_cols);
	Map<MatrixXd> C(
	    m_mixing_matrix.matrix, m_mixing_matrix.num_rows,
	    m_mixing_matrix.num_cols);

	// Unmix
	EX = C.inverse() * EX;

	return new CDenseFeatures<float64_t>(X);
}

CFeatures* CICAConverter::inverse_transform(CFeatures* features, bool inplace)
{
	REQUIRE(m_mixing_matrix.matrix, "ICAConverter has not been fitted.\n");

	auto X = features->as<CDenseFeatures<float64_t>>()->get_feature_matrix();
	if (!inplace)
		X = X.clone();

	linalg::matrix_prod(m_mixing_matrix, X, X);

	return new CDenseFeatures<float64_t>(X);
}
