#ifndef CAMERA_H
#define CAMERA_H

#include "QMatrix4.h"
#include "QVector3.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A first-person viewpoint of a viewing volume. Supports orthographic and
 perspective projects and simple character movement. This camera expects
 world-space coordinates -- and does not account for transformations from
 model to world space.
 */
class Camera {
  public:
    /**
     Creates a camera sitting at the origin and facing down the positive
     z-axis. Objects within the unit cube surrounding the viewer are mapped
     to the viewport.
     */
    Camera();

    /**
     Creates a camera sitting at <code>from</code>, looking at <code>to</code>,
     with the specified <code>up</code> vector. Objects within the unit cube
     surrounding the viewer are mapped to the viewport.
     @param from Camera position in world space
     @param to Focal location in world space
     @param up General up direction in world space
     @see LookAt
     */
    Camera(const QVector3<float>& from,
           const QVector3<float>& to,
           const QVector3<float>& up);

    /**
     Gets the camera's location in world space.
     @return Camera's world space location
     */
    const QVector3<float>& GetFrom() const;
    const QVector3<float>& GetTo() const;
    const QVector3<float>& GetUp() const;

    QVector3<float> GetForward() const;
    QVector3<float> GetRight() const;
    QVector3<float> GetRealUp() const;

    /**
     Gets the transformation matrix that will transform to a system where the
     camera is at the origin, whose view direction is the positive z-axis, whose
     right-vector is the x-axis, and whose adjusted up-vector is the y-axis.
     @return Camera transform
     */
    const QMatrix4<float>& GetViewMatrix() const;

    /**
     Gets the transformation matrix that will transform this camera's viewing
     frame to x in [-1, 1], y in [-1 1], and z in [-1 1].
     @return Projection transform
     */
    const QMatrix4<float>& GetProjectionMatrix() const;

    /**
     Shifts the camera's position and look target by the specified offsets.
     @param delta_x x-offset
     @param delta_y y-offset
     @param delta_z z-offset
     */
    void Move(float delta_x,
              float delta_y,
              float delta_z);

    /**
     Adjusts the camera to sit at <code>from</code>, look at <code>to</code>,
     and be roughly oriented according to specified <code>up</code> vector.
     @param from Camera position in world space
     @param to Focal location in world space
     @param up General up direction in world space
     */
    void LookAt(const QVector3<float>& from,
                const QVector3<float>& to,
                const QVector3<float>& up);

    /**
     Sets the projection to the map the specified bounding volume to the
     canonical viewing volume.
     @param l Left bounds, x == l maps to -1
     @param r Right bounds, x == r maps to 1
     @param b bottom bounds, y == b maps to -1
     @param t top bounds, y == t maps to 1
     @param n near bounds, z == n maps to -1
     @param f far bounds, z == f maps to 1
     @see QMatrix4::GetOrthographic
     */
    void SetOrthographic(float l,
                         float r,
                         float b,
                         float t,
                         float n,
                         float f);

    /**
     Sets the projection to map the specified bounding volume centered around
     the camera to the canonical viewing volume. The bounding volume is
     automatically expanded to maintain the specified aspect ratio. It is never
     shrunk.
     @param min_width Minimum width of bounding box
     @param min_height Minimum height of bounding box
     @param aspect Width-height ratio
     @param n near bounds, z == n maps to -1
     @param f far bounds, z == f maps to 1
     @see QMatrix4::GetOrthographic
     */
    void SetOrthographic(float min_width,
                         float min_height,
                         float aspect,
                         float n,
                         float f);

    /**
     Sets the projection to map the specified bounding volume to the canonical
     viewing volume. Perspective is applied such that objects farther from the
     near clipping plane will be scaled down.
     @param l Left bounds of near clipping plane, x == l, z == n maps to -1
     @param r Right bounds of near clipping plane, x == r, z == n maps to 1
     @param b bottom bounds of near clipping plane, y == b, z == n maps to -1
     @param t top bounds of near clipping plane, y == t, z == n maps to 1
     @param n near bounds, z == n maps to -1
     @param f far bounds, z == f maps to 1
     @see QMatrix4::GetPerspective(T, T, T, T, T, T)
     */
    void SetPerspective(float l,
                        float r,
                        float b,
                        float t,
                        float n,
                        float f);

    /**
     Sets the projection to map the specified bounding volume to the canonical
     viewing volume. Perspective is applied such that objects farther from the
     near clipping plane will be scaled down.
     @param fov_y Field of view of bounding volume, in degrees, along y-axis
     @param aspect Aspect ratio of bounding volume
     @param near z-value that maps to -1.
     @param far z-value that maps to 1.
     @see QMatrix4::GetPerspective(T, T, T, T)
     */
    void SetPerspective(float fov_y,
                        float aspect,
                        float near,
                        float far);

    /**
     Rolls the camera around its z-axis.
     @param degrees Amount to rotate
     */
    void Roll(float degrees);

    /**
     Pitches the camera around its x-axis.
     @param degrees Amount to rotate
     */
    void Pitch(float degrees);

    /**
     Yaws the camera around its y-axis.
     @param degrees Amount to rotate
     */
    void Yaw(float degrees);

    /**
     Advances the camera along its look direction.
     @param delta Amount to advance, negative to retreat
     */
    void Advance(float delta);

    /**
     Shifts the camera left or right.
     @param delta Amount to strafe, positive to move right, negative to move left
     */
    void Strafe(float delta);

    /**
     Move the camera's from and look positions y-position to the specified value.
     @param height New height
     */
    void ElevateTo(float height);

    void SetTilt(float offset);

    void SwitchToOrthographic();
    void SwitchToPerspective();
    
    void SetVerticalFieldOfView(float fov_y);
    float GetVerticalFieldOfView() const;
    float GetHorizontalFieldOfView(float aspect_ratio) const;

    float GetPerspectiveNear() const;
    float GetPerspectiveFar() const;
    float GetOrthographicNear() const;
    float GetOrthographicFar() const;

    void SetPerspectiveNear(float near);
    void SetPerspectiveFar(float far);
    void SetOrthographicNear(float near);
    void SetOrthographicFar(float far);

    void SetOrthographicLeft(float l);
    void SetOrthographicRight(float r);
    void SetOrthographicBottom(float b);
    void SetOrthographicTop(float t);

  private:
    /** View transform */
    QMatrix4<float> view;

    /** Project transform */
    QMatrix4<float> perspective_projection;
    QMatrix4<float> orthographic_projection;

    /** Direction to focus location */
    QVector3<float> focal_direction;

    /** Camera's position in world space */
    QVector3<float> from;

    /** Camera's focus in world space */
    QVector3<float> to;

    /** Camera's general up vector; may need adjustment to maintain orthogonality */
    QVector3<float> up;

    /** World-space x-axis, useful for strafing */
    QVector3<float> right;

    float tilt;
    
    float perspective_frustum[6];

    float orthographic_frustum[6];

    float fov_y;

    float aspect_ratio;

    bool is_perspective;
};

/* ------------------------------------------------------------------------- */

inline const QMatrix4<float>& Camera::GetViewMatrix() const {
  return view;
}

/* ------------------------------------------------------------------------- */

inline const QMatrix4<float>& Camera::GetProjectionMatrix() const {
  return is_perspective ? perspective_projection : orthographic_projection;
}

/* ------------------------------------------------------------------------- */

}

#endif
