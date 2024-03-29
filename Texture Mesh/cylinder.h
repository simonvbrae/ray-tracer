#pragma once
#include <math.h> 
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"

class cylinder : public object {

public:
	cylinder() : y0(-1), y1(1), radius(1), inv_radius(1) {};

	cylinder(const material *m) : y0(-1), y1(1), radius(1), inv_radius(1) {};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec, float time) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max, float time) const;
	virtual void u_v_calculate(hit_record& rec) const;

	virtual bool bounding_box(aabb& box) const;
private:

	float		y0;						// bottom y value
	float		y1;						// top y value
	float		radius;					// radius
	float		inv_radius;  			// one over the radius	
};

bool cylinder::bounding_box(aabb& box) const {
	box = aabb(point3D(0, y0, 0) - vector3D(radius, 0, radius), point3D(0, y1, 0) + vector3D(radius, 0, radius));
	return true;
}

bool cylinder::hit(const ray& ray, float t_min, float t_max, hit_record& rec, float time) const {
	float temp;
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;

	float a = dx * dx + dz * dz;
	float b = 2.0f * (ox * dx + oz * dz);
	float c = ox * ox + oz * oz - radius * radius;
	float disc = b * b - 4.0f * a * c;

	if (disc < 0.0)
		return(false);
	else {
		float e = sqrt(disc);
		float denom = 2.0f * a;
		temp = (-b - e) / denom;    // smaller root

		if (temp < t_max && temp > t_min) {
			float yhit = oy + temp * dy;

			if (yhit > y0 && yhit < y1) {
				rec.t = temp;
				rec.normal = normalize(vector3D((ox + temp * dx) * inv_radius, 0.0f, (oz + temp * dz) * inv_radius));
				rec.p = ray.point_at_parameter(rec.t);
				//rec.m = mat;

				// test for hitting from inside
				if (dot(-ray.d, rec.normal) < 0.0f)
					rec.normal = -rec.normal;

				u_v_calculate(rec);
				return (true);
			}
		}

		temp = (-b + e) / denom;    // larger root
		
		if (temp < t_max && temp > t_min) {
			float yhit = oy + temp * dy;

			if (yhit > y0 && yhit < y1) {
				rec.t = temp;
				rec.normal = normalize(vector3D((ox + temp * dx) * inv_radius, 0.0f, (oz + temp * dz) * inv_radius));
				rec.p = ray.point_at_parameter(rec.t);
				//rec.m = mat;

				// test for hitting inside surface
				if (dot(-ray.d, rec.normal) < 0.0f)
					rec.normal = -rec.normal;
				u_v_calculate(rec);
				return (true);
			}
		}
	}

	return (false);
}

float mi = 40;
float ma = -100;
void cylinder::u_v_calculate(hit_record& rec) const {
	float azimuth = atan(rec.p.x / rec.p.z) + M_PI / 2;
	rec.u = azimuth;
	if (rec.u < mi || rec.u > ma) {
		if (rec.u > ma) {
			ma = rec.u;
		}
		else {
			mi = rec.u;
		}
	}
	rec.u = (rec.u) / (M_PI);
	rec.v = (rec.p.y + 1) / 2;
}


bool cylinder::hit_shadow(const ray& ray, float t_min, float t_max, float time) const {
	float temp;
	float ox = ray.o.x;
	float oy = ray.o.y;
	float oz = ray.o.z;
	float dx = ray.d.x;
	float dy = ray.d.y;
	float dz = ray.d.z;

	float a = dx * dx + dz * dz;
	float b = 2.0f * (ox * dx + oz * dz);
	float c = ox * ox + oz * oz - radius * radius;
	float disc = b * b - 4.0f * a * c;

	if (disc < 0.0f)
		return(false);
	else {
		float e = sqrt(disc);
		float denom = 2.0f * a;
		temp = (-b - e) / denom;    // smaller root

		if (temp < t_max && temp > t_min) {
			float yhit = oy + temp * dy;

			if (yhit > y0 && yhit < y1) {
				//rec.t = temp;
				//rec.normal = normalize(vector3D((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius));

				// test for hitting from inside

				//if (dot(-ray.d, rec.normal) < 0.0)
				//	rec.normal = -rec.normal;

				//rec.p = ray.point_at_parameter(rec.t);

				return (true);
			}
		}

		temp = (-b + e) / denom;    // larger root

		if (temp < t_max && temp > t_min) {
			float yhit = oy + temp * dy;

			if (yhit > y0 && yhit < y1) {
				//rec.t = temp;
				//rec.normal = normalize(vector3D((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius));

				// test for hitting inside surface

				//if (dot(-ray.d, rec.normal) < 0.0)
				//	rec.normal = -rec.normal;

				//rec.p = ray.point_at_parameter(rec.t);

				return (true);
			}
		}
	}

	return (false);
}
