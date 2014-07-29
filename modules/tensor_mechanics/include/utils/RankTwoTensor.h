#ifndef RANKTWOTENSOR_H
#define RANKTWOTENSOR_H

#include "Moose.h"

// Any requisite includes here
#include "libmesh/libmesh.h"
#include "libmesh/vector_value.h"
#include "libmesh/tensor_value.h"

#include <vector>


/**
 * RankTwoTensor is designed to handle the Stress or Strain Tensor for a fully anisotropic material.
 * It is designed to allow for maximum clarity of the mathematics and ease of use.
 * Original class authors: A. M. Jokisaari, O. Heinonen, M. R. Tonks
 *
 * RankTwoTensor holds the 9 separate Sigma_ij or Epsilon_ij entries.
 * The entries are accessed by index, with i, j equal to 1, 2, or 3, or
 * internally i, j = 0, 1, 2.
 */
class RankTwoTensor
{
public:

  /// Default constructor; fills to zero
  RankTwoTensor();

  /**
   * Constructor that takes in 3 vectors and uses them to create rows
   * _vals[0][i] = row1(i), _vals[1][i] = row2(i), _vals[2][i] = row3(i)
   */
  RankTwoTensor(const TypeVector<Real> & row1, const TypeVector<Real> & row2, const TypeVector<Real> & row3);

  /// Constructor that proxies the fillFromInputVector method
  RankTwoTensor(const std::vector<Real> & input) { this->fillFromInputVector(input); };

  /// Initialization list replacement constructors, 6 arguments
  RankTwoTensor(Real S11, Real S22, Real S33, Real S23, Real S13, Real S12);

  /// Initialization list replacement constructors, 9 arguments
  RankTwoTensor(Real S11, Real S21, Real S31, Real S12, Real S22, Real S32, Real S13, Real S23, Real S33);

  /// Copy constructor from RankTwoTensor
  RankTwoTensor(const RankTwoTensor & a);

  /// Copy constructor from RealTensorValue
  RankTwoTensor(const TypeTensor<Real> & a);

  /// Gets the value for the index specified.  Takes index = 0,1,2
  Real & operator()(unsigned int i, unsigned int j);

  /// Gets the value for the index specified.  Takes index = 0,1,2, used for const
  Real operator()(unsigned int i, unsigned int j) const;

  /// zeroes all _vals components
  void zero();

  /**
  * fillFromInputVector takes 6 or 9 inputs to fill in the Rank-2 tensor.
  * If 6 inputs, then symmetry is assumed S_ij = S_ji, and
  *   _vals[0][0] = input[0]
  *   _vals[1][1] = input[1]
  *   _vals[2][2] = input[2]
  *   _vals[1][2] = input[3]
  *   _vals[0][2] = input[4]
  *   _vals[0][1] = input[5]
  * If 9 inputs then input order is [0][0], [1][0], [2][0], [0][1], [1][1], ..., [2][2]
  */
  void fillFromInputVector(const std::vector<Real> & input);

public:
  /// returns _vals[r][i], ie, row r, with r = 0, 1, 2
  TypeVector<Real> row(const unsigned int r) const;

  /**
   * rotates the tensor data given a rank two tensor rotation tensor
   * _vals[i][j] = R_ij * R_jl * _vals[k][l]
   * @param R rotation matrix as a RealTensorValue
   */
  virtual void rotate(RealTensorValue & R);

  /**
   * rotates the tensor data given a rank two tensor rotation tensor
   * _vals[i][j] = R_ij * R_jl * _vals[k][l]
   * @param R rotation matrix as a RankTwoTensor
   */
  virtual void rotate(RankTwoTensor & R);

  /**
   * rotates the tensor data anticlockwise around the z-axis
   * @param a angle in radians
   */
  virtual RankTwoTensor rotateXyPlane(const Real a);

  /**
   * Returns a matrix that is the transpose of the matrix this
   * was called on.
   */
  RankTwoTensor transpose() const;

  /// sets _vals to a, and returns _vals
  RankTwoTensor & operator= (const RankTwoTensor & a);

  /// adds a to _vals
  RankTwoTensor & operator+= (const RankTwoTensor & a);

  /// returns _vals + a
  RankTwoTensor operator+ (const RankTwoTensor & a) const;

  /// sets _vals -= a and returns vals
  RankTwoTensor & operator-= (const RankTwoTensor & a);

  /// returns _vals - a
  RankTwoTensor operator- (const RankTwoTensor & a) const;

  /// returns -_vals
  RankTwoTensor operator - () const;

  /// performs _vals *= a
  RankTwoTensor & operator*= (const Real & a);

  /// returns _vals*a
  RankTwoTensor operator* (const Real & a) const;

  /// performs _vals /= a
  RankTwoTensor & operator/= (const Real & a);

  /// returns _vals/a
  RankTwoTensor operator/ (const Real & a) const;

  /// performs _vals *= a (component by component) and returns the result
  RankTwoTensor & operator*= (const RankTwoTensor & a);

  /// Defines multiplication with another RankTwoTensor
  RankTwoTensor operator* (const RankTwoTensor & a) const;

  /// Defines multiplication with a TypeTensor<Real>
  RankTwoTensor operator* (const TypeTensor<Real> & a) const;

  /// returns _vals_ij * a_ij (sum on i, j)
  Real doubleContraction(const RankTwoTensor & a);

  /**
   * Denote the _vals[i][j] by A_ij, then
   * S_ij = A_ij - de_ij*tr(A)/3
   * Then this returns S_ij*S_ij/2
   */
  Real secondInvariant() const;


  /// returns A_ij - de_ij*tr(A)/3, where A are the _vals
  RankTwoTensor deviatoric() const;


  /**
   * Denote the _vals[i][j] by A_ij, then this returns
   * d(trace)/dA_ij
   */
  RankTwoTensor dtrace() const;

  /**
   * Denote the _vals[i][j] by A_ij, then this returns
   * d(secondInvariant)/dA_ij
   */
  RankTwoTensor dsecondInvariant() const;

  /**
   * Denote the _vals[i][j] by A_ij, then this returns
   * d(det)/dA_ij
   */
  RankTwoTensor ddet() const;


  /// returns the trace of the tensor, ie _vals[i][i] (sum i = 0, 1, 2)
  Real trace() const;

  /// Calculate the determinant of the tensor
  Real det() const;

  /// Calculate the inverse of the tensor
  RankTwoTensor inverse() const;

  /// Print the rank two tensor
  void print() const;

  /// Add identity times a to _vals
  void addIa(const Real & a);

  /// Sqrt(_vals[i][j]*_vals[i][j])
  Real L2norm() const;

  /**
   * sets _vals[0][0], _vals[0][1], _vals[1][0], _vals[1][1] to input,
   * and the remainder to zero
   */
  void surfaceFillFromInputVector(const std::vector<Real> & input);



protected:


private:
  static const unsigned int N = 3;
  Real _vals[N][N];
};

inline RankTwoTensor operator*(Real a, const RankTwoTensor & b) { return b * a; }

#endif //RANKTWOTENSOR_H
